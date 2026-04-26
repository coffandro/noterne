build:
	make -C src

watch:
	make build
	while inotifywait -r -e modify,create,delete src; do \
		make build; \
	done