
# Uso: ./crearTests l_kVecinos l_alfaDims l_Kpliegues PCA cantImgsEntrenamiento rutaDatos
# Por ejemplo: ./crearTests 3 15 10 0 410 ../data/train.csv

rutaDatos="$6"

nomCarpetaNuevosTests='tests_recien_creados'

mkdir "$nomCarpetaNuevosTests"

for kVecinos in $1; do

	for alfaDims in $2; do

		for Kpliegues in $3; do

			for nroExperimento in 1 2 3; do

				if [[ $4 == 0 ]]; then
					nomArchivo="$nomCarpetaNuevosTests"'/test_k'$kVecinos'a'$alfaDims'K'$Kpliegues'-'$nroExperimento'.in'
				else
					nomArchivo="$nomCarpetaNuevosTests"'/test_k'$kVecinos'a'$alfaDims'K'$Kpliegues'PCA-'$nroExperimento'.in'
				fi

				echo "$rutaDatos" $kVecinos $alfaDims $Kpliegues > "$nomArchivo"

				matlab -nojvm -nodesktop -r "particionarValidX($Kpliegues, '$nomArchivo', $5); quit;" && reset         # el "reset" es para des-buguear la terminal (Matlab te la buguea)
				sleep 100
				#octave-cli --eval "particionarValidX($Kpliegues, '$nomArchivo', $5); quit;"	# listos o no: migramos a octave. venció la licencia de matlab en la facu
			done
		done
	done
done

# ordenar

cd $nomCarpetaNuevosTests

for kVecinos in $1; do

        mkdir -p k$kVecinos

        mv test*k$kVecinos*.in k$kVecinos

        for alfaDims in $2; do

                mkdir -p k$kVecinos/a$alfaDims

                mv k$kVecinos/test*a$alfaDims*.in k$kVecinos/a$alfaDims

                for Kpliegues in $3; do

                        mkdir -p k$kVecinos/a"$alfaDims"/K$Kpliegues

                        mv k$kVecinos/a$alfaDims/test*K$Kpliegues*.in k$kVecinos/a$alfaDims/K$Kpliegues
                done

        done

done

