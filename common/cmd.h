#ifndef __CMD_H__
#define __CMD_H__

#include <assert.h>

#define OPT_SHORT(_i, _opts, _field, _name)                                             \
	if (StrEquals(argv[_i], _name)) {                                                     \
		i++;                                                                                \
		assert(i < argc);                                                                   \
		_opts._field = argv[i];                                                             \
	}

#endif // __CMD_H__
