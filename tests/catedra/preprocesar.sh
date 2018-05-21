
# agrega "../data/" a cada línea y cambia la extensión .in por .csv

cd $(dirname $0)

for archivo in *.in; do

    sed -e 's#^#../data/#' "${archivo}" > `echo preprocesado_$archivo | sed 's/\(.*\.\)in/\1csv/'`
done
