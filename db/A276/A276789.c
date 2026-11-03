/*
 * A276789  First differences of A003145.
 * https://oeis.org/A276789
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A003145(n+1) - A003145(n)
 * Range:     u8 n <= 10608, u16 n <= 10608, u32 n <= 10608, u64 n <= 10608,
 *            u128 n <= 10608, big any n
 * Validated: standard, pass, 116 terms through n = 9943 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is the fixed point of the morphism 4->43, 3->42, 2->4 */
/* Generated lazily and memoised for repeated calls. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../utils/mem.h"

/* C allocation helpers */
static seq_t* seq_alloc_new_array(size_t n)   { return mem_new(seq_t, n); }

static void seq_free_array(seq_t* p)          { mem_free(p); }

seq_t a(idx_t n)
{
    static seq_t *memo = NULL;   /* buffer holding the generated terms   */
    static size_t length = 0;    /* number of terms stored in memo       */

    /* initialize on first call */
    if (memo == NULL) {
        memo   = seq_alloc_new_array(1);
        memo[0] = (seq_t)4;
        length = 1;
    }

    /* Expand the word until we have at least n terms */
    while (length < (size_t)n) {
        size_t new_cap = length * 2;                 /* enough for maximal expansion */
        seq_t *new_buf = seq_alloc_new_array(new_cap);
        size_t pos = 0;

        for (size_t i = 0; i < length; ++i) {
            if (memo[i] == (seq_t)4) {               /* 4 -> 4 3 */
                new_buf[pos++] = (seq_t)4;
                new_buf[pos++] = (seq_t)3;
            } else if (memo[i] == (seq_t)3) {        /* 3 -> 4 2 */
                new_buf[pos++] = (seq_t)4;
                new_buf[pos++] = (seq_t)2;
            } else if (memo[i] == (seq_t)2) {        /* 2 -> 4   */
                new_buf[pos++] = (seq_t)4;
            }                                     /* other values never occur     */
        }

        seq_free_array(memo);
        memo   = new_buf;
        length = pos; /* actual length after expansion */
    }

    return memo[(size_t)(n - 1)];
}
