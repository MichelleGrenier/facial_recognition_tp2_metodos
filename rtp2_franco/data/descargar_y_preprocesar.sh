wget http://www-2.dc.uba.ar/materias/metnum/dnload/2017_C2/tp2/data.zip
unzip data.zip
tail -n +2 train.csv > train.csv.tmp  &&  mv train.csv.tmp train.csv

