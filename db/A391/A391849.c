/*
 * A391849  Odd numbers k such that 13*k = A048720(m, k) for some m, where
 *          A048720 is carryless base-2 multiplication.
 * https://oeis.org/A391849
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 328, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bitlen_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence: odd k with 13*k = A048720(m,k) for some m.
 *  Computed by checking that the carryless binary polynomial of
 *  13*k is divisible by that of k over GF(2).
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/bitlen_seq_t.h"

seq_t a(idx_t n)
{
    /* cache of found terms and next odd candidate to test */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static seq_t next_candidate = (seq_t)1;   /* start at 1, odd */

    /* Ensure we have at least n terms computed */
    while (cache_len < (size_t)n) {
        seq_t k = next_candidate;           /* candidate odd k */
        seq_t prod = k * (seq_t)13;          /* 13 * k as integer */

        /* Polynomial division of prod by k over GF(2) */
        seq_t temp = prod;
        seq_t divisor = k;
        unsigned dlen = bitlen_seq_t(divisor);

        while (temp != (seq_t)0) {
            unsigned tlen = bitlen_seq_t(temp);
            if (tlen < dlen) break;
            unsigned shift = tlen - dlen;
            temp ^= divisor << shift;
        }

        if (temp == (seq_t)0) {               /* k is in the sequence */
            if (cache_len == cache_cap) {      /* grow cache */
                size_t new_cap = (cache_cap == 0) ? 8 : cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) {
                    free(cache);
                    abort();                   /* unrecoverable error */
                }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = k;
        }

        next_candidate += (seq_t)2;           /* skip even numbers */
    }

    return cache[(size_t)(n - 1)];
}
