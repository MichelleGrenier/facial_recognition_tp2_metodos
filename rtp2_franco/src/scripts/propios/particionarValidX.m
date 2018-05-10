function particionarValidX(Kpliegues, rutaArchivoSalida)

pkg load statistics		% octave-cli

cantImgsEntrenamiento=42000;

fid=fopen(rutaArchivoSalida,'a');

c = cvpartition(cantImgsEntrenamiento,'kfold',Kpliegues);

for i = 1:Kpliegues
	a = training(c,i);
	b(i,:) = a';
end

dlmwrite(rutaArchivoSalida, b, '-append', 'delimiter', ' ');

fclose(fid);

