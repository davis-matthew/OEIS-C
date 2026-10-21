/*
 * A273618  Numbers m = 2*k+1 where k is odd with the property that 3^k mod m
 *          = 1 and k^k mod m = 1.
 * https://oeis.org/A273618
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 822, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search form: a(n) is the n-th odd integer m=2*k+1 with odd k
 * such that 3^k ≡ 1 (mod m) and k^k ≡ 1 (mod m).  */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    /* Cache of found terms */
    static seq_t *terms = NULL;
    static size_t terms_len = 0, terms_cap = 0;

    /* Next odd k to test */
    static idx_t next_k = 1;

    /* Extend cache until it holds at least n terms */
    while (terms_len < n) {
        /* Ensure next_k is odd */
        if ((next_k & 1) == 0)
            ++next_k;

        seq_t m = ((seq_t)next_k) * 2 + 1;

        /* Test the two congruences */
        if (mod_pow((seq_t)3, (uint64_t)next_k, m) == (seq_t)1 &&
            mod_pow((seq_t)next_k, (uint64_t)next_k, m) == (seq_t)1) {

            /* Grow cache if needed */
            if (terms_len == terms_cap) {
                terms_cap = terms_cap ? terms_cap << 1 : 32;
                terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
            }
            terms[terms_len++] = m;
        }
        ++next_k;
    }

    return terms[n - 1];
}
