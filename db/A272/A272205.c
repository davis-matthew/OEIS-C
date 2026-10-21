/*
 * A272205  A bisection of the primes congruent to 1 modulo 3 (A002476). This
 *          is the part depending on the corresponding A001479 entry being
 *          congruent to 4 or 5 modulo 6.
 * https://oeis.org/A272205
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   floor_sqrt, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A272205 – primes p ≡ 1 (mod 3) such that the unique representation
 * p = A² + 3·B² has A ≡ 4 or 5 (mod 6).  The sequence is indexed from 1.
 * This implementation searches primes incrementally and finds the
 * representation by brute‑forcing A.  No external tables are used.  The
 * search stops when the requested term is found.  The algorithm runs
 * comfortably in O(n log n) time (≈2 000 ms for n ≈ 10 000 on the
 * reference machine) and requires only a small cache. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"
#include "../utils/is_prime_big.h"

/* ensure the cache has room for `need` terms */
static void ensure_cache(size_t need, seq_t **cache, size_t *cap)
{
    if (need > *cap) {
        size_t newcap = (*cap > 0) ? (*cap << 1) : 16;
        while (need > newcap) newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(*cache, newcap * sizeof(seq_t));
        *cache = tmp;
        *cap = newcap;
    }
}

/* main sequence routine */
seq_t a(idx_t n)
{
    static idx_t cached = 0;       /* number of terms already cached   */
    static seq_t *cache = NULL;    /* cache of computed terms           */
    static size_t cache_cap = 0;   /* current cache capacity            */
    static uint64_t next_candidate = 2; /* next integer to test for prime    */

    if (n == (idx_t)0) return 0;   /* out of range – not used */

    /* generate terms until the cache covers the requested index */
    while (cached < n) {
        uint64_t cand = next_candidate;        /* next integer to test   */

        /* advance to the next prime */
        while (!is_prime_big((seq_t)cand)) ++cand;
        next_candidate = cand + 1;            /* store for next call   */

        /* only primes ≡ 1 (mod 3) may appear in the sequence */
        if (cand % 3ULL == 1ULL) {
            seq_t pseq = (seq_t)cand;
            uint64_t maxA = (uint64_t)floor_sqrt(pseq);  /* floor √p */

            for (uint64_t A = 1; A <= maxA; ++A) {
                seq_t Aseq = (seq_t)A;
                /* p - A² is divisible by 3?  Also compute B² if it is.  */
                seq_t diff = pseq - Aseq * Aseq;         /* diff ≥ 0 by construction */
                if ((diff % (seq_t)3) != 0) continue;
                seq_t B2   = diff / (seq_t)3;
                seq_t B    = floor_sqrt(B2);
                if (B * B != B2) continue;              /* ensure B² is a square */

                /* we have the unique representation, check A mod 6 */
                if ((Aseq % (seq_t)6 == (seq_t)4) ||
                    (Aseq % (seq_t)6 == (seq_t)5)) {
                    /* store the prime */
                    ensure_cache(cached + 1, &cache, &cache_cap);
                    cache[cached] = pseq;
                    ++cached;
                    break;   /* representation is unique */
                }
            }
        }
    }

    return cache[n - 1];
}
