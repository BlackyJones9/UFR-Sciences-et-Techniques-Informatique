.PHONY: clean dist
id = damiamat

dist: clean
	tar -hzcf "../../$(id).tar.gz" ../projet/* ../rapport.pdf
clean:
	$(MAKE) -C jaccard clean
