#ifndef __CMD_H__
#define __CMD_H__

#include <assert.h>

#define OPT_SHORT(_i, _opts, _field, _name)                                             \
	if (StrEquals(argv[_i], _name)) {                                                     \
		i++;                                                                                \
		assert(i < argc);                                                                   \
		_opts._field = argv[i];                                                             \
	}

#define OPT_LONG(_i, _opts, _field, _short_name, _long_name)                            \
	if (StrEquals(argv[_i], _short_name) || StrEquals(argv[_i], _long_name)) {            \
		i++;                                                                                \
		assert(i < argc);                                                                   \
		_opts._field = argv[i];                                                             \
	}

#define FLAG_SHORT(_i, _opts, _field, _name)                                            \
	if (StrEquals(argv[_i], _name)) {                                                     \
		_opts._field = true;                                                                \
	}

#define FLAG_LONG(_i, _opts, _field, _short_name, _long_name)                           \
	if (StrEquals(argv[_i], _short_name) || StrEquals(argv[_i], _long_name)) {            \
		_opts._field = true;                                                                \
	}

#endif // __CMD_H__
