.PHONY: clean up install

clean:
	@rm -r output

up:
	@vagrant up

acceptance: up
	@vagrant ssh --command "/vagrant/build.sh"
