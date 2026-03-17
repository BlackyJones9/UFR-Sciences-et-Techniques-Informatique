.PHONY: clean dist

dist: clean
	tar -hzcf "$(CURDIR).tar.gz" hashtable/* project_main/* holdall/* makefile

clean:
	$(MAKE) -C project_main clean
