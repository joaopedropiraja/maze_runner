#ifndef MSGASSERT_H
#define MSGASSERT_H

#define _GET_FIRST_ARG(arg1, ...) arg1
#define _IS_EMPTY(...) _GET_FIRST_ARG(__VA_ARGS__, 0)

#define _msgassert_impl(cond, msg, ...) \
  if (!(cond)) \
    _assert(#cond, __FILE__, __LINE__, msg, ##__VA_ARGS__)

#define _msgassert_no_msg(cond) \
  if (!(cond)) \
    _assert(#cond, __FILE__, __LINE__, "")

#define _msgassert_select(cond, msg, ...) \
  _msgassert_impl(cond, msg, ##__VA_ARGS__)

#define msgassert(cond, ...) \
  _msgassert_select(cond, _GET_FIRST_ARG(__VA_ARGS__, ""), ##__VA_ARGS__)

void _assert (const char* snippet, const char* file, int line, const char* message, ...);

#endif // MSGASSERT_H