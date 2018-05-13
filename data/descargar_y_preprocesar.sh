
# descargar

	## viejo
		# wget http://www-2.dc.uba.ar/materias/metnum/dnload/2017_C2/tp2/data.zip && \
		# no están más acá los datos viejos :c
		# pero se pueden bajar de acá https://www.kaggle.com/c/digit-recognizer/data
		# la paja: necesitás hacerte cuenta y en bugmenot y un par de páginas más, no encontramos nada

	## nuevo
		echo ""
		echo "Bajar los datos nuevos de acá:"
		echo "https://campus.exactas.uba.ar/pluginfile.php/79576/course/section/12820/archivos_tp2.zip"
		echo "(Da un toque más laburo automatizar la descarga con el campus nuevo, así que por ahora es a mano)"
		echo ""
		echo "No hace falta inciar sesión, se puede ingresar como invitado."
		echo ""
		echo "Es un zip con:"
		echo " * los pdf del enunciado y la presentación del tp"
		echo " * un zip con las imágenes (completas y reducidas)"
		echo " * y las diapos de la intro a manejo de imágenes"
		#echo " * y un zip con el código para manejo de imágenes (ignorar: ya está subido al repo)"
		echo ""
		echo "Los tests de la cátedra no hace falta bajarlos, ya están subidos al repo."
		echo "Estos tests asumen que las imágenes completas están en un directorio llamado \"full\" y las reducidas en un directorio llamado \"reduced\"."
		echo ""
		echo "El informe depende del enunciado (lo incluye en un apéndice)."
		echo "Así que para compilar el informe, el enunciado tiene que estar en el directorio raíz del repo."
		echo ""

# descomprimir

	## viejo
		# (unzip data.zip && rm data.zip) && \

	## nuevo
		# (unzip archivos_tp2.zip && rm archivos_tp2.zip) && \
		# ( (unzip ImagenesCaras.zip && rm ImagenesCaras.zip); \
		#  (unzip ImagenesCarasRed.zip && rm ImagenesCarasRed.zip) )

# pre-procesar

	## viejo
		# tail -n +2 train.csv > train.csv.tmp  &&  mv train.csv.tmp train.csv

	## nuevo

		### necesario

			# mv ImagenesCaras full
			# mv ImagenesCarasRed reduced
			# mv enunciadoTP2.pdf ../
		
		### opcional

			# mv intro_imagenes.pdf ../
			# mv presTP2.pdf ../

