/*
 * A285688  a(1) = 2; a(n + 1) = smallest prime > a(n) such that a(n + 1) -
 *          a(n) is the product of three primes.
 * https://oeis.org/A285688
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   count_prime_factors, grow_seq, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/count_prime_factors.h"
#include "../utils/grow_seq.h"

/* a(1) = 2;
   a(n+1) = the smallest prime > a(n) such that the difference a(n+1)-a(n)
   is the product of exactly three (not necessarily distinct) primes. */

seq_t a(idx_t n)
{
    /* Cache of computed terms.  Indices are 1‑based. */
    static seq_t *cache = NULL;
    static size_t cap   = 0;   /* allocated slots in cache      */
    static size_t computed = 0;/* number of terms already in cache */

    /* initialise cache with the first term */
    if (computed == 0) {
        cap = 1;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        cache[0] = (seq_t)2;
        computed = 1;
    }

    /* grow the cache if we need to compute more terms */
    while (computed < (size_t)n) {
        seq_t cur = cache[computed - 1];
        seq_t cand = cur + (seq_t)1;
        seq_t next;

        /* find the next term */
        for (;;) {
            if (!is_prime_big(cand)) {
                ++cand;
                continue;
            }
            seq_t diff = cand - cur;
            seq_t pf = count_prime_factors(diff);
            if (pf == (seq_t)3) {
                next = cand;
                break;
            }
            ++cand;
        }

        /* ensure capacity for the new term */
        if (computed >= cap) {
            grow_seq(&cache, &cap, computed + 1);
        }

        cache[computed] = next;
        ++computed;
    }

    return cache[n - 1];
}
