/*
 * A285691  a(1) = 2; a(n + 1) = smallest prime > a(n) such that a(n + 1) -
 *          a(n) is the product of six primes.
 * https://oeis.org/A285691
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=2; for n>1, a(n) is the smallest prime > a(n‑1) with
 *            (a(n)-a(n‑1)) having exactly six prime factors counting
 *            multiplicity
 * Range:     u8 n <= 1, u16 n <= 358, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bigomega, grow_seq, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/next_prime.h"
#include "../utils/bigomega.h"
#include "../utils/grow_seq.h"

seq_t a(idx_t n)
{
    /* Use 1‑based indexing as in OEIS.  n==0 is out of range; return 0. */
    if (n == 0)
        return (seq_t)0;

    /* Static cache grows as needed. */
    static seq_t *cache      = NULL;
    static idx_t  cache_last = 0;   /* highest index computed so far   */
    static size_t cache_cap  = 0;   /* current array capacity          */

    /* Ensure the cache can hold up to index n (inclusive). */
    if (cache_last < n)
    {
        /* Grow to hold indices 0..n (we ignore index 0). */
        grow_seq(&cache, &cache_cap, (size_t)(n + 1));

        /* Compute missing terms. */
        for (idx_t i = cache_last + 1; i <= n; ++i)
        {
            if (i == 1)
            {
                cache[1] = (seq_t)2;
                continue;
            }

            seq_t prev = cache[i - 1];
            /* Find the next prime greater than prev. */
            uint64_t cand_val = next_prime((uint64_t)prev);
            seq_t cand = (seq_t)cand_val;

            /* Keep searching until the difference has exactly 6 prime factors. */
            while (bigomega((idx_t)(cand - prev)) != 6)
            {
                cand_val = next_prime(cand_val + 1);
                cand = (seq_t)cand_val;
            }

            cache[i] = cand;
        }

        cache_last = n;
    }

    return cache[n];
}
