function particionarValidX(Kpliegues, rutaArchivoSalida, cantImgsEntrenamiento)

%pkg load statistics		% octave-cli

fid=fopen(rutaArchivoSalida,'a');

c = cvpartition(cantImgsEntrenamiento,'kfold',Kpliegues);

for i = 1:Kpliegues
	a = training(c,i);
	b(i,:) = a';
end

dlmwrite(rutaArchivoSalida, b, '-append', 'delimiter', ' ');

fclose(fid);

