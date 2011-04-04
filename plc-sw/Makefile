all:
	@echo "Please enter in what you want to make."
	@echo "- test_startup: test to see if the board starts up"
	@echo "- clean"

test_system: arch_pic16
	cd test/system; make

test_startup: test_system
	cp test/system/test_startup.hex target.hex

test_io: test_system
	cp test/system/test_io.hex target.hex

uip:
	cd uip; make

arch_pic16:
	cd arch/pic16; make

update_tags:
	ctags -R .
clean:
	cd test/system; make clean
	cd arch/pic16; make clean
	rm -f target.hex

install:
	sudo pk2cmd -PPIC18F4550 -A3.3 -R -E -M -Ftarget.hex -B/usr/share/pk2/

install_powered:
	sudo pk2cmd -PPIC18F4550 -A3.3 -M -Ftarget.hex -B/usr/share/pk2/
