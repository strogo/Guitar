
#ifndef UNICODE_H_
#define UNICODE_H_

#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <functional>

namespace unicode_helper_ {

struct utf8_reader_state_t {
	int a;
	uint32_t b;
};

class utf8decoder {
private:
	char const *begin;
	char const *end;
	size_t pos;

	utf8_reader_state_t s;
public:
	utf8decoder(char const *begin, char const *end);
	uint32_t next();
};

}

class abstract_unicode_reader;

class utf8encoder {
private:
	struct internal_data {
		abstract_unicode_reader *reader;
		char c;
		char buf[8];
		int pos, len;
	} data;

	class internal_writer;

	void set(abstract_unicode_reader *reader);
	bool next();
	inline bool next_();
public:
	utf8encoder(abstract_unicode_reader *reader = 0);
	char get();
};

class utf16encoder {
private:
	struct internal_data {
		abstract_unicode_reader *reader;
		uint16_t c;
		uint16_t buf[2];
		int pos, len;
	} data;

	class internal_writer;

	void set(abstract_unicode_reader *reader);
	bool next();
	bool next_();
public:
	utf16encoder(abstract_unicode_reader *reader = 0);
	uint16_t get();
};


class abstract_unicode_reader {
public:
	virtual ~abstract_unicode_reader()
	{
	}
	virtual uint32_t next() = 0;

	void to_utf8(std::function<bool(char)> fn)
	{
		utf8encoder e(this);
		while (1) {
			int c = e.get();
			if (c == 0) break;
			if (!fn(c)) break;
		}
	}
	void to_utf16(std::function<bool(uint16_t)> fn)
	{
		utf16encoder e(this);
		while (1) {
			int c = e.get();
			if (c == 0) break;
			if (!fn(c)) break;
		}
	}
	void to_utf32(std::function<bool(uint32_t)> fn)
	{
		while (1) {
			uint32_t c = next();
			if (c == 0) break;
			if (!fn(c)) break;
		}
	}
};

class utf32 : public abstract_unicode_reader {
private:
	struct {
		uint32_t const *ptr;
		uint32_t const *end;
	} data;
public:
	utf32(uint32_t const *ptr, uint32_t const *end);
	utf32(uint32_t const *ptr);
	utf32(uint32_t const *ptr, size_t len);
	uint32_t next();
};

class utf16 : public abstract_unicode_reader {
private:
	struct {
		uint16_t const *ptr;
		uint16_t const *end;
	} data;
public:
	utf16(uint16_t const *ptr, uint16_t const *end);
	utf16(uint16_t const *ptr);
	utf16(uint16_t const *ptr, size_t len);
	uint32_t next();
};

class utf8 : public abstract_unicode_reader {
private:
	unicode_helper_::utf8decoder reader;
public:
	utf8(char const *ptr, char const *end);
	utf8(char const *ptr);
	utf8(char const *ptr, size_t len);
	uint32_t next();
};


#endif
