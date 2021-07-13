import json
from nltk.corpus import stopwords
from nltk.tokenize import word_tokenize
import re
from nltk.stem import WordNetLemmatizer
from nltk.stem import LancasterStemmer
import csv

def listToString(s): 
    str1 = "" 
    for ele in s: 
        if ele == s[len(s)-1]:
            str1 = str1 + ele + '$'
        else:
            str1 = str1 + ele + ''  
    return str1 

#extraer datos necesarios
with open('output.json') as f:
  data = json.load(f)

ids = []
titulos = []
abstracts = []
for line in data:
    ids.append(line['id'])
    titulos.append(line['title'])
    abstracts.append(line['abstract'])

#remover caracteres innecesarios y stop words
stop_words = set(stopwords.words('english'))

titulos2 = []
for titulo in titulos:
    titulos2.append(" ".join(re.findall("[a-zA-Z]+", titulo.lower())))

abstracts2 = []
for abstract in abstracts:
    abstracts2.append(" ".join(re.findall("[a-zA-Z]+", abstract.lower())))

abstracts3 = []
for abstract2 in abstracts2:
    word_tokens = word_tokenize(abstract2)
    filtered_sentence = [w for w in word_tokens if not w.lower() in stop_words]
    filtered_sentence = []
    for w in word_tokens:
        if w not in stop_words:
            filtered_sentence.append(w)
    abstracts3.append(filtered_sentence)

#stem
wordnet_lemmatizer = WordNetLemmatizer()
lancaster=LancasterStemmer()

abstracts4 = []
for abstract3 in abstracts3:
    provisional = []
    for palabra in abstract3:
        #provisional.append(lancaster.stem(palabra))
        provisional.append(wordnet_lemmatizer.lemmatize(palabra))
    abstracts4.append(provisional)

#CREAR csv
with open('datosBase_lema.csv', 'w', encoding='UTF8', newline='') as f:
    # create the csv writer
    writer = csv.writer(f)

    # write a row to the csv file
    for i in range(0, len(ids)):
        row = [ids[i], titulos2[i], listToString(abstracts4[i])]
        writer.writerow(row)

print(ids[0])
print(titulos2[0])
print(abstracts4[0])