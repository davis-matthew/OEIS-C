/*
 * A339910  After 1, numbers k > 1 such that k has fewer prime divisors than
 *          k-1, when they are counted with multiplicity.
 * https://oeis.org/A339910
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 104, u16 n <= 9481, u32 n <= 9481, u64 n <= 9481,
 *            u128 n <= 9481, big any n
 * Helpers:   bigomega, grow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A339910: numbers k > 1 such that bigomega(k) < bigomega(k-1),
 * with a(1)=1.  The implementation keeps a cache of found terms and
 * extends it as needed.  bigomega is a utility that returns the
 * number of prime factors (counted with multiplicity) of an idx_t.
 * grow_seq ensures that the cache has enough space.  O(n) time for
 * the nth term (n ≤ 10000). */
#include "../utils/grow_seq.h"
#include "../utils/bigomega.h"

seq_t a(idx_t n)
{
    /* N == 1 is the special first term. */
    if (n == 1)
        return (seq_t)1;

    /* Static state for lazy caching. */
    static seq_t *cache = NULL;      /* cached sequence values */
    static size_t cache_cap = 0;     /* allocated capacity */
    static size_t cached_terms = 0;  /* number of terms stored */
    static idx_t next_k = 2;         /* next integer to test */

    /* Initialise the cache with the first term if not done yet. */
    if (cached_terms == 0) {
        grow_seq(&cache, &cache_cap, 1);
        cache[0] = (seq_t)1;
        cached_terms = 1;
    }

    /* Return from cache if already computed. */
    if ((idx_t)n <= cached_terms)
        return cache[n - 1];

    /* Extend the cache until we have the nth term. */
    while (cached_terms < n) {
        /* Search for the next integer k satisfying the condition. */
        while (1) {
            seq_t omega_k     = bigomega(next_k);
            seq_t omega_k_minus = bigomega(next_k - 1);
            if (omega_k < omega_k_minus) {
                /* Found a new term.  Append it to the cache. */
                grow_seq(&cache, &cache_cap, cached_terms + 1);
                cache[cached_terms] = (seq_t)next_k;
                cached_terms++;
                next_k++;
                break;
            }
            next_k++;
        }
    }

    return cache[n - 1];
}
