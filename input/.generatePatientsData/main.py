

import csv
import random
from unidecode import unidecode
import kaggle
import os
os.chdir('input/.generatePatientsData')

class NameGenerator():
    # Source: https://www.behindthename.com/api/download.php
    def __init__(self, names_file_name = 'btn_givennames.txt', sure_file_name = 'btn_surnames.txt'):
        n = open(names_file_name, 'r', encoding='utf-8')
        s = open(sure_file_name, 'r', encoding='utf-8')
        
        # Check if the files were found
        if(n == None or s == None):
            raise Exception("File not found")
        
        # Read the names and surnames 
        self.male_names = []
        self.female_names = []
        for line in n.readlines()[5:]:
            if(line[0] == '#'): # skip the comments
                continue
            line = line.strip().split('\t')
            
            if 'm' in line[1]: # if the name is masculine
                self.male_names.append(line[0])
            if 'f' in line[1]: # if the name is feminine
                self.female_names.append(line[0])

        self.surnames = s.readlines()[5:] # skip the first 5 lines of the file
    
    # Generate a name based on gender and id
    def generate_name(self, gender = None, id = None):
        if id == None:
            id = random.randint(0, min(len(self.male_names), len(self.female_names)))
        if gender == None:
            gender = random.choice(['m', 'f'])
        elif(gender != 'm' and gender != 'f'):
            raise Exception("Invalid gender")
        if gender == 'm':
            return self.male_names[id % len(self.male_names)]
        else:
            return self.female_names[id % len(self.female_names)]

    # Generate a surname based on id
    def generate_surname(self, id = None):
        if id == None:
            id = random.randint(0, len(self.surnames))
        return self.surnames[id % len(self.surnames)].strip()
    
    # Generate a full name returning a string without special characters
    def generate(self, gender = None, id = None):
        return unidecode(self.generate_name(gender, id) + ' ' + self.generate_surname(id))
        
        
if __name__ == "__main__":
    # Source: https://www.kaggle.com/joniarroba/noshowappointments
    kaggle.api.dataset_download_files('joniarroba/noshowappointments', path='data/', unzip=True)
    source = open('data/KaggleV2-May-2016.csv', 'r', encoding='utf-8')
    
    src = csv.DictReader(source)

    name = NameGenerator()
    
    output_Patients = open('../Patients.csv', 'w', encoding='utf-8', newline='')
    patients = csv.writer(output_Patients)
    patients.writerow(['id', 'nome', 'sobrenome', 'genero', 'dia_nascimento', 'mes_nascimento', 'ano_nascimento', 'bairro', 'hipertensao', 'diabetes', 'alcoolismo'])
    patients_ids = []

    print('Generating Patients...')
    for row in src:
        # Generate patient data
        gender = row['Gender'].lower()
        id = int(row['PatientId'].replace('E-', ''))
        if id in patients_ids:
            continue # Patient already exists
        patients_ids.append(id)
        name_parts = name.generate(gender, id).split()
        patients.writerow([
            id, 
            name_parts[0], 
            name_parts[1],
            row['Gender'],
            random.randint(1, 28),
            random.randint(1, 12),
            2024 - int(row['Age']),
            row['Neighbourhood'],
            row['Hipertension'],
            row['Diabetes'],
            row['Alcoholism']
        ])
        
    output_Patients.close()
    print(f'Generated {len(patients_ids)} Patients')