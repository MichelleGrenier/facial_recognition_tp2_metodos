
# agrega "../data/" a cada línea y cambia la extensión .in por .csv

cd $(dirname $0)    # para que se corra desde la carpeta en la que está (y no la carpeta desde donde lo llaman)

for archivo in *.in; do

    sed -e 's#^#../data/#' "${archivo}" > `echo preprocesado_$archivo | sed 's/\(.*\.\)in/\1csv/'`
done

