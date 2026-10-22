/*
 * A380882  Centered square numbers which are sphenic numbers.
 * https://oeis.org/A380882
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth centered square number that is a sphenic number.
 *            Centered square numbers: C(m) = 2*m^2 - 2*m + 1 A number is
 *            sphenic if it is the product of exactly three distinct primes
 * Range:     u8 none, u16 n <= 25, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for 0, but harmless */

    /* Static cache of found terms and progress */
    static seq_t *terms   = NULL;
    static size_t terms_cap = 0;
    static size_t terms_len = 0;
    static idx_t last_m   = 0;      /* last centered-square index processed */

    /* Grow or initialise cache if needed */
    if (terms == NULL) {
        terms_cap = 16;
        terms = (seq_t *)malloc(terms_cap * sizeof(seq_t));
    }

    /* Generate terms until we have n terms */
    while (terms_len < n) {
        ++last_m;                       /* next centered square index */

        seq_t mseq = (seq_t)last_m;
        seq_t C    = (seq_t)2 * mseq * mseq - (seq_t)2 * mseq + 1;

        /* Factor the value (fits in 64 bits for the n values tested) */
        uint64_t val   = (uint64_t)C;
        uint64_t primes[64];
        uint64_t exps[64];
        size_t cnt = 0;
        factor_u64(val, primes, exps, &cnt);

        /* Check for a sphenic number: exactly 3 distinct primes,
           each occurring to the first power. */
        if (cnt == 3 && exps[0] == 1 && exps[1] == 1 && exps[2] == 1) {
            if (terms_len == terms_cap) {
                size_t new_cap = terms_cap * 2;
                seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
                if (tmp == NULL) { /* allocation failure – terminate */
                    return (seq_t)0;
                }
                terms = tmp;
                terms_cap = new_cap;
            }
            terms[terms_len++] = C;
        }
    }

    /* Return the requested term (1‑based indexing). */
    return terms[n - 1];
}
