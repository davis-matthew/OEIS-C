/*
 * A346114  a(n+1) = 1 + Sum_{k=1..n} a(gcd(n,k)).
 * https://oeis.org/A346114
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + Sum_{d|n-1} φ((n-1)/d) * a(d) (offset 1)
 * Range:     u8 n <= 15, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1)=1.  Recurrence uses divisors of n-1 and Euler’s totient φ.  */
/* The function caches values so that repeated calls, even with out-of-order
   arguments, run in O(n + n log log n) time for n up to about 10 000.  */
/* All arithmetic is performed using the generic seq_t type.  */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static idx_t max_computed = 0;          /* largest index stored in cache */
    /* static totient array */
    static uint32_t *phi = NULL;
    static idx_t phi_cap = 0;               /* maximum index computed in phi */

    if (n == 0) return 0; /* safety, not used in OEIS offset */

    /* Make sure cache can hold index n */
    if (max_computed < n) {
        idx_t new_cap = n + 1; /* indices 0..n inclusive */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0; /* allocation failure */
        cache = tmp;
        /* initialise freshly allocated part to zero */
        for (idx_t i = max_computed + 1; i <= n; ++i) cache[i] = 0;
    }

    /* Initialise a(1) if needed */
    if (max_computed == 0) {
        cache[1] = 1;
        max_computed = 1;
    }

    /* Ensure φ array is large enough (needs indices up to n‑1) */
    if (phi_cap <= n - 1) {
        idx_t need = n; /* allocate indices 0..n-1 inclusive */
        uint32_t *tmp = (uint32_t *)realloc(phi, need * sizeof(uint32_t));
        if (!tmp) return 0;
        phi = tmp;
        for (idx_t i = 0; i < need; ++i) phi[i] = i;
        /* compute φ for all values < need */
        for (idx_t i = 2; i < need; ++i) {
            if (phi[i] == i) {               /* i is prime */
                for (idx_t j = i; j < need; j += i) {
                    phi[j] -= phi[j] / i;
                }
            }
        }
        phi_cap = need;
    }

    /* Compute missing terms up to n */
    for (idx_t m = max_computed + 1; m <= n; ++m) {
        idx_t k = m - 1;                     /* divisor argument */
        seq_t sum = 1;                       /* the leading '1' in recurrence */
        for (idx_t d = 1; d * d <= k; ++d) {
            if (k % d == 0) {
                idx_t d1 = d;
                idx_t d2 = k / d;
                sum += (seq_t)phi[k / d1] * cache[d1];
                if (d1 != d2) {
                    sum += (seq_t)phi[k / d2] * cache[d2];
                }
            }
        }
        cache[m] = sum;
    }
    max_computed = n;
    return cache[n];
}
