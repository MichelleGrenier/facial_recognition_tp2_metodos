function particionarValidX(Kpliegues, rutaArchivoSalida, cantImgsEntrenamiento)

	isOctave = exist('OCTAVE_VERSION', 'builtin') ~= 0;

	if (isOctave)
		pkg load statistics
	end

	fid=fopen(rutaArchivoSalida,'a');

	c = cvpartition(cantImgsEntrenamiento,'kfold',Kpliegues);

	for i = 1:Kpliegues
		a = training(c,i);
		b(i,:) = a';
	end

	dlmwrite(rutaArchivoSalida, b, '-append', 'delimiter', ' ');

	fclose(fid);

end

