import numpy as np
import pandas as pd
from sklearn.impute import SimpleImputer
from sklearn.model_selection import train_test_split
from sklearn.ensemble import RandomForestClassifier
from sklearn.metrics import accuracy_score

fine_propeller = pd.read_csv('fine_propeller_vibr_0.csv', header=None, sep=';')
one_wing = pd.read_csv('one_wing_vibr_0.csv', header=None, sep=';')
half_wing_broke = pd.read_csv('half_wing_broke_vibr_0.csv', header=None, sep=';')
half_wing_broke['label'] = 2
fine_propeller['label'] = 0
one_wing['label'] = 1
data = pd.concat([fine_propeller, one_wing, half_wing_broke], ignore_index=True)

# Usunięcie kolumn zawierających wyłącznie wartości NaN
data = data.dropna(axis=1, how='all') 

X = data.drop('label', axis=1)
y = data['label']

# zastąpienia wartości NaN medianą
imputer = SimpleImputer(strategy='median')
X_imputed = imputer.fit_transform(X)


X_train, X_test, y_train, y_test = train_test_split(X_imputed, y, test_size=0.2, random_state=42)

# Trening modelu
clf = RandomForestClassifier()
clf.fit(X_train, y_train)


y_pred = clf.predict(X_test)
print("Dokładność:", accuracy_score(y_test, y_pred)*100, "%")


clf = RandomForestClassifier()
clf.fit(X_imputed, y)

def classify_new_data(file_path):
    try:
        new_data = pd.read_csv(file_path, header=None, sep=';')
        new_data = new_data.dropna(axis=1, how='all')
        new_data_imputed = imputer.transform(new_data)
        
        prediction = clf.predict(new_data_imputed)
        
        
        # Sprawdzenie, czy dane są podobne do 'fine_propeler' czy 'one_wing'
        if prediction[0] == 0:
            return "Data are similar to fine_propeler."
        if prediction[0] == 1:
            return "Data are similar to one_wing."
        else:
            return "Data are similar to half_wing_broke."
            
    except Exception as e:
        return f"Data classification error: {e}"

# Wgrywanie piluku do oceny
result = classify_new_data("fine_propeller_vibr_1.csv")
print(result)
