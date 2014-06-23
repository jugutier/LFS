COMPILE=compile
$(COMPILE):
	@gcc -o filesystem main.c fs.c driver.c applications.c external_string_lib.c
