import cs50
import csv

from flask import Flask, jsonify, redirect, render_template, request

# Configure application
app = Flask(__name__)

# Reload templates when they are changed
app.config["TEMPLATES_AUTO_RELOAD"] = True


@app.after_request
def after_request(response):
    """Disable caching"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/", methods=["GET"])
def get_index():
    return redirect("/form")


@app.route("/form", methods=["GET"])
def get_form():
    return render_template("form.html")


@app.route("/form", methods=["POST"])
def post_form():
    name = request.form.get("inputName")
    drink = request.form.get("drinks")
    side = request.form.get("sides")
    size = request.form.get("size")
    if not name:
        return render_template("error.html", message="You forgot your name!")
    elif not drink:
        return render_template("error.html", message="You forgot your drink!")
    elif not side:
        return render_template("error.html", message="Please choose none if not wanted!")

    csvfile = open('survey.csv', 'w', newline='')
    orderlist = csv.writer(csvfile)
    orderlist.writerow([name] + [drink] + [size] + [side])
    csvfile.close()
    return redirect("/sheet")

@app.route("/sheet", methods=["GET"])
def get_sheet():
    file = open("survey.csv", "r")
    reader = csv.reader(file)
    data = list(reader)
    return render_template("sheet.html", data=data)
