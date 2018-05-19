import os
from utils import listfiles

# Sources listadas manualmente
# sources = ['main', 'utils', 'utils2/utils2']

# Sources listadas automaticamente
sources = [f.rstrip('.cpp') for f in listfiles('.', '*.cpp')]

# Compilador
compiler = 'g++'

# Programa compilado
executable = './tp2' if os.name == 'posix' else 'tp2.exe'

# Parametros extra
extraParams = []

