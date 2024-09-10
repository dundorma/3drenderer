build:
	nix-build default.nix

run:
	./result/bin/renderer

clean:
	rm result
	nix-collect-garbage --delete-older-than 7d
