from nltk.tokenize import sent_tokenize

def lines(a, b):
    """Return lines in both a and b"""
    line1 = a.splitlines()
    line2 = b.splitlines()
    result = []
    for line in line1:
        if line not in result and line in line2:
            result.append(line)
    return result

def sentences(a, b):
    """Return sentences in both a and b"""
    sent1 = sent_tokenize(a, language='english')
    sent2 = sent_tokenize(b, language='english')
    result = []
    for line in sent1:
        if line not in result and line in sent2:
            result.append(line)
    return result

def substrings(a, b, n):
    """Return substrings of length n in both a and b"""
    sub1 = []
    sub2 = []
    result = []
    for i in range(len(a) - n + 1):
        sub1.append(a[i:(i + n)])
    for i in range(len(b) - n + 1):
        sub2.append(b[i:(i + n)])
    for line in sub1:
        if line in sub2 and line not in result:
            result.append(line)
    return result
