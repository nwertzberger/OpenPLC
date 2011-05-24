/* Programmer: Nicholas Wertzberger
 *      Email: wertnick@gmail.com
 *
 * All work Copyright 2010 Nicholas Wertzberger.
 *
 * The following are debug macros intended to help find problems when
 * debugging.
 */
#ifndef CORE_DEBUG_H
#define CORE_DEBUG_H

/* The assert operation.  Right now it doesn't have a good way to show
 * there's a problem... will fix ASAP.
 */
#ifdef DEBUG
#   define ASSERT(x) if(!(x)) {     \
    _asm                            \
    CLEAR                           \
    GOTO 0                          \
    _endasm;}
#else
#   define ASSERT(x)
#endif

#endif

