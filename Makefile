.PHONY: clean up acceptance

clean:
	@rm -r output

up:
	@vagrant up

acceptance: up
	@vagrant ssh --command "/vagrant/build.sh"
