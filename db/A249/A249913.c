/*
 * A249913  Numbers in accordance with alphabet placement on a qwerty
 *          keyboard.
 * https://oeis.org/A249913
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 26, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A249913: numbers in accordance with alphabet placement
   on a QWERTY keyboard. The sequence is periodic with period 26.
   a(n) = mapping[(n-1) % 26] where mapping is the table below. */
#include <stddef.h>   /* for size_t */

seq_t a(idx_t n)
{
    static const seq_t map[26] = {
        2,  /* A */
        6,  /* B */
        4,  /* C */
        4,  /* D */
        7,  /* E */
        5,  /* F */
        6,  /* G */
        7,  /* H */
        17, /* I */
        8,  /* J */
        9,  /* K */
        0,  /* L */
        8,  /* M */
        7,  /* N */
        9,  /* O */
        0,  /* P */
        3,  /* Q */
        9,  /* R */
        3,  /* S */
        11, /* T */
        15, /* U */
        5,  /* V */
        5,  /* W */
        3,  /* X */
        13, /* Y */
        2   /* Z */
    };

    /* n is guaranteed to be ≥ 1 by the specification. */
    size_t idx = (size_t)((n - 1) % 26);
    return map[idx];
}
