COMPILE=compile
$(COMPILE):
	@gcc -o filesystem main.c fs/fs.c diskDriver/driver.c applications.c external_string_lib.c
