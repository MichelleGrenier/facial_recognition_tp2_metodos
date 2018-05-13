report.pdf: report.tex sections/*.tex ../enunciadoTP2.pdf logo_uba.jpg logo_dc.jpg caratula.sty
	pdflatex report.tex && pdflatex report.tex # need to compile twice to make the index

.PHONY: clean
clean:
	rm -f report.pdf *.aux *.log *.toc *.out sections/*.aux sections/*.log sections/*.toc sections/*.out

