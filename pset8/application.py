import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response

# Custom filter
app.jinja_env.filters["usd"] = usd
app.jinja_env.globals.update(usd=usd)

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():
    userInfo = db.execute("SELECT symbol, SUM(share), SUM(total) FROM purchases WHERE userId = :id GROUP BY symbol HAVING SUM(share) > 0", id = session["user_id"])
    balance = db.execute("SELECT cash FROM users WHERE Id = :id", id = session["user_id"])
    totalHold = 0
    for row in userInfo:
        quote = lookup(row["symbol"])
        row["price"] = quote["price"]
        #quoteSum in usd
        quoteSum = quote["price"] * row["SUM(share)"]
        #totalHold in usd
        totalHold += quoteSum

    totalHold += balance[0]["cash"]
    return render_template("index.html", userInfo = userInfo, balance = balance, totalHold = totalHold)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    if request.method == "POST":
        symbol = request.form.get("symbol")
        shares = request.form.get("shares")
        quote = lookup(symbol)

        #server-side check for validity of symbol
        if not quote:
            return apology("Invalid symbol")

        #server-side check for validity of shares
        elif not shares.isdigit():
            return apology("Shares must be a positive whole number")

        #check if user has enough money to buy said shares
        else:
            cost = int(shares) * float(quote["price"])
            userCash = db.execute("SELECT cash FROM users WHERE id = :id", id = session["user_id"])
            userCash = float(userCash[0]["cash"])
            cashLeft = userCash - cost
            if userCash < cost:
                return apology("You don't have enough money")
            #database logs
            else:
                db.execute("UPDATE users SET cash = :cash where id = :id", cash = cashLeft, id = session["user_id"])
                db.execute("INSERT INTO purchases (userId, symbol, share, total) VALUES(:userId, :symbol, :share, :total)",
                userId = session["user_id"], symbol = quote["symbol"], share = shares, total = cost)

        return redirect("/")
    else:
        return render_template("buy.html")


@app.route("/check", methods=["GET"])
def check():
    username = request.args.get("username")
    if len(username) > 0 and len(db.execute("SELECT username FROM users WHERE username = :username", username = username)) == 0:
        return jsonify(True)
    else:
        return jsonify(False)


@app.route("/history")
@login_required
def history():
    buyHist = db.execute("SELECT symbol, share, total, timeIn FROM purchases where userId = :id AND share > 0 GROUP BY timeIn", id = session["user_id"])
    saleHist = db.execute("SELECT symbol, share, total, timeIn FROM purchases where userId = :id AND share < 0 GROUP BY timeIn", id = session["user_id"])
    return render_template("history.html", buyHist = buyHist, saleHist = saleHist)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    if request.method == "POST":
        quoted = lookup(request.form.get("symbol"))
        if not quoted:
            return apology("No such symbol")
        else:
            return render_template("quoted.html", quote=quoted)
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    if request.method == "POST":

        name = request.form.get("username")
        if not name:
            return apology("name required")

        password = request.form.get("password")
        if not password:
            return apology("password required")

        confirmation = request.form.get("confirmation")
        if not confirmation:
            return apology("please retype password")
        if password != confirmation:
            return apology("passwords don't match bruh")
        else:
            password = generate_password_hash(password)
            result = db.execute("INSERT INTO users (username,hash) VALUES(:name, :password)", name=name, password=password)
            if not result:
                return apology("username is taken")
            return redirect("/")
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    if request.method == "POST":
        # get user input and quote of said stock
        userNum = int(request.form.get("shares"))
        userSym = request.form.get("symbol")
        quote = lookup(userSym)
        # reject user shares if more than what he has
        shares = db.execute("SELECT SUM(share) FROM purchases WHERE userId = :id AND symbol = :symbol", id = session["user_id"], symbol = userSym)
        if userNum > shares[0]["SUM(share)"]:
            return apology("you don't have enough stocks to sell")
        # calculate sale price
        sale = userNum * float(quote["price"])
        # calculate balance
        balance = db.execute("SELECT cash FROM users WHERE id = :id", id = session["user_id"])
        total = balance[0]["cash"] + sale
        # modify database
        db.execute("UPDATE users SET cash = :cash WHERE id = :id", cash = total, id = session["user_id"])
        db.execute("INSERT INTO purchases (userId, symbol, share, total) VALUES(:userId, :symbol, :share, :total)",
        userId = session["user_id"], symbol = userSym, share = (-userNum), total = sale)
        return redirect("/")
    else:
        # shows user currently owned stock symbols
        symbol = db.execute("SELECT symbol FROM purchases WHERE userId = :id GROUP BY symbol HAVING SUM(share) > 0", id = session["user_id"])
        return render_template("sell.html", symbol=symbol)

@app.route("/change", methods=["GET", "POST"])
@login_required
def change():
    if request.method == "POST":
        oldPass = db.execute("SELECT hash FROM users WHERE id = :id", id = session["user_id"])
        print(oldPass[0]["hash"])
        # userinput
        password = request.form.get("oldPass")
        if not password:
            return apology("please type in old password")
        elif not check_password_hash(oldPass[0]["hash"], password):
            return apology("your password is wrong")
        newPass = request.form.get("newPass")
        if not newPass:
            return apology("please type in a new password")
        confirmation = request.form.get("confirmation")
        if confirmation != newPass:
            return apology("your new passwords do not match")

        # check if new password is different
        if check_password_hash(oldPass[0]["hash"], newPass):
            return apology("your new password is the same")
        else:
            passHash = generate_password_hash(newPass)
            db.execute("UPDATE users SET hash = :phash WHERE id = :id", phash = passHash, id = session["user_id"])
        return redirect("/")
    else:
        return render_template("change.html")





def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
