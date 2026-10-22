/*
 * A391397  Numbers k such that d(k+1)/d(k) = 2^m for some integer m
 *          (positive, zero, or negative), where d(k) is the number of
 *          divisors of k.
 * https://oeis.org/A391397
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   A391397 – numbers k such that d(k+1)/d(k) is a power of 2
 *            (positive, zero, or negative). The function returns the n‑th
 *            term of this sequence (offset 1)
 * Range:     u8 n <= 128, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>
#include "../utils/divisor_count_u64.h"

seq_t a(idx_t n)
{
    assert(n >= 1);                         /* offset is 1 */

    /* static cache of found terms */
    static seq_t *terms = NULL;
    static idx_t cap   = 0;
    static idx_t len   = 0;
    static uint64_t next_k = 1;

    /* initialise cache once */
    if (terms == NULL) {
        cap = 16;
        terms = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!terms) abort();
    }

    /* generate terms until we have at least n of them */
    while (len < n) {
        uint64_t k   = next_k;
        seq_t d_k    = divisor_count_u64(k);
        seq_t d_k1   = divisor_count_u64(k + 1);

        bool ok = false;
        if (d_k == d_k1) {
            ok = true;                     /* ratio = 1 = 2^0 */
        } else if (d_k < d_k1) {
            if (d_k1 % d_k == 0) {
                seq_t q = d_k1 / d_k;
                if ((q & (q - 1)) == 0)     /* power of two */
                    ok = true;
            }
        } else {                            /* d_k > d_k1 */
            if (d_k % d_k1 == 0) {
                seq_t q = d_k / d_k1;
                if ((q & (q - 1)) == 0)
                    ok = true;
            }
        }

        if (ok) {
            /* resize cache if needed */
            if (len == cap) {
                idx_t new_cap = cap * 2;
                seq_t *tmp = (seq_t*)malloc(new_cap * sizeof(seq_t));
                if (!tmp) abort();
                for (idx_t i = 0; i < len; ++i) tmp[i] = terms[i];
                free(terms);
                terms = tmp;
                cap   = new_cap;
            }
            terms[len++] = (seq_t)k;        /* store the term */
        }
        ++next_k;
    }

    return terms[n - 1];
}
