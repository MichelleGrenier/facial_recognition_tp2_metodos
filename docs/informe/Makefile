informe.pdf: informe.tex secciones/*.tex ../enunciadoTP2.pdf logo_uba.jpg logo_dc.jpg caratula.sty
	pdflatex informe.tex && pdflatex informe.tex # need to compile twice to make the index

.PHONY: clean
clean:
	rm -f informe.pdf *.aux *.log *.toc *.out secciones/*.aux secciones/*.log secciones/*.toc secciones/*.out

