for archivo in *.in; do

    sed -e 's#^#../data/#' "${archivo}" > preprocesado_"${archivo}"
done
