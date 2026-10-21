/*
 * A254158  Decimal expansion of alpha particle-proton mass ratio.
 * https://oeis.org/A254158
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = nth digit of the decimal expansion of the alpha
 *            particle–proton mass ratio, which equals 4001506179127 /
 *            1007276466621
 * Range:     u8 none, u16 none, u32 none, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence starts with: 3,9,7,2,5,9,9,6,9,0,... */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *buf = NULL;    /* digits array */
    static idx_t cap = 0;        /* allocated capacity */
    static idx_t len = 0;        /* number of digits stored */
    static seq_t rem = 0;        /* current remainder */

    /* Constants as 64‑bit integers converted to seq_t */
    const seq_t NUM = (seq_t)4001506179127ULL;
    const seq_t DEN = (seq_t)1007276466621ULL;

    if (n == 0) return 0; /* undefined for n=0, return 0 safely */

    /* First call: initialize integer part and capacity */
    if (len == 0) {
        seq_t int_part = NUM / DEN;
        rem = NUM % DEN;

        cap = (n > 20 ? n : 20); /* start with a reasonable size */
        buf = (seq_t*)malloc(cap * sizeof(seq_t));
        buf[0] = int_part;
        len = 1;
    }

    /* If more digits are requested, compute them */
    if (n > len) {
        /* Grow buffer if necessary */
        if (n > cap) {
            while (cap < n) cap <<= 1;              /* double until enough */
            buf = (seq_t*)realloc(buf, cap * sizeof(seq_t));
        }

        for (idx_t i = len; i < n; ++i) {
            rem = rem * 10;               /* shift remainder */
            seq_t d = rem / DEN;          /* next digit */
            rem = rem % DEN;              /* new remainder */
            buf[i] = d;
        }
        len = n;
    }

    return buf[n - 1];
}
