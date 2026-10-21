/*
 * A277590  Numbers k such that k/10^m == 3 mod 10, where 10^m is the greatest
 *          power of 10 that divides n.
 * https://oeis.org/A277590
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 7284, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: generate all numbers whose right‑most non‑zero digit is 3 */
#include <stdlib.h>
/* seq_t, idx_t are supplied by the build system */

seq_t a(idx_t n)
{
    static seq_t *terms = NULL;      /* cached sequence values */
    static size_t terms_len = 0;     /* number of terms stored */
    static size_t terms_cap = 0;     /* allocated capacity */
    static seq_t next_int = 1;       /* next integer to test */

    if (n == 0)          /* undefined, but guard against */
        return 0;

    /* generate terms until we have at least n of them */
    while ((idx_t)terms_len < n) {
        /* test the current candidate */
        seq_t tmp = next_int;
        /* strip trailing zeros */
        while ((tmp % 10) == 0)
            tmp /= 10;
        if ((tmp % 10) == 3) {
            /* store the term */
            if (terms_len == terms_cap) {
                size_t new_cap = terms_cap ? terms_cap * 2 : 16;
                seq_t *new_arr = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
                if (!new_arr)
                    abort(); /* memory allocation failure */
                terms = new_arr;
                terms_cap = new_cap;
            }
            terms[terms_len++] = next_int;
        }
        ++next_int;   /* advance to next integer */
    }

    return terms[n - 1];
}
