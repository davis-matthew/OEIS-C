/*
 * A395574  1 = Sum_{n>=0} a(n) * x^n * (1 - (2*n+1)*x)^(n+1).
 * https://oeis.org/A395574
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = Σ_{k=1..⌊(n+1)/2⌋} (-1)^(k-1) * (2n-2k+1)^k *
 *            binomial(n-k+1,k) * a(n-k)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 21,
 *            big any n
 * Helpers:   big_pow, binomial, ensure_cache, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/binomial.h"
#include "../utils/big_pow.h"
#include "../utils/seq_mul.h"
#include "../utils/ensure_cache.h"

/* Computes the n‑th term of OEIS sequence A395574. */
seq_t a(idx_t n)
{
    /* Static cache for previously computed values */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t computed_max = 0;   /* largest index that has been computed */

    /* Ensure the cache can hold index n (at least n+1 entries). */
    ensure_cache(n, &cache, &cache_cap);

    /* Initialise a(0) if it has not yet been set. */
    if (cache[0] == 0) {
        cache[0] = 1;
    }

    /* Return cached value if already computed. */
    if (n <= computed_max) {
        return cache[n];
    }

    /* Compute terms from computed_max+1 up to n. */
    for (idx_t t = computed_max + 1; t <= n; ++t) {
        seq_t pos_sum = 0;
        seq_t neg_sum = 0;
        idx_t kmax = (t + 1) / 2;
        for (idx_t k = 1; k <= kmax; ++k) {
            /* (2*t - 2*k + 1)^k */
            seq_t base = (seq_t)(2 * t - 2 * k + 1);
            seq_t pow_term = big_pow(base, (seq_t)k);

            /* binomial(t - k + 1, k) */
            seq_t bin = binomial(t - k + 1, k);

            /* a(t - k) from cache */
            seq_t prev = cache[t - k];

            /* product = pow_term * bin * prev */
            seq_t prod = seq_mul(seq_mul(pow_term, bin), prev);

            if (k & 1) {          /* k odd -> + */
                pos_sum += prod;
            } else {              /* k even -> - */
                neg_sum += prod;
            }
        }
        /* Compute the result as the difference of the two sums. */
        if (pos_sum < neg_sum) {
            cache[t] = neg_sum - pos_sum;
        } else {
            cache[t] = pos_sum - neg_sum;
        }
    }

    computed_max = n;
    return cache[n];
}
