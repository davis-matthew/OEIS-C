/*
 * A360548  E.g.f. satisfies A(x) = x * exp( 2*(x + A(x)) ).
 * https://oeis.org/A360548
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} (2*k)^(n-1) * binomial(n,k) (OEIS A360548)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   binomial, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/binomial.h"   /* provides seq_t binomial(idx_t n, idx_t k) */
#include "../utils/pow_seq.h"    /* provides seq_t pow_seq(seq_t base, int exp) */

seq_t a(idx_t n)
{
    /* Handle trivial case */
    if (n == 0)
        return (seq_t)0;

    /* Static state for memoisation */
    static seq_t *cache   = NULL;   /* a(k) for k <= current_n      */
    static seq_t *pow_arr = NULL;   /* (2*k)^(current_n-1) for k<=current_n */
    static idx_t  current_n = 0;    /* largest n already computed     */

    /* Expand storage if needed */
    if (n > current_n) {
        /* Allocate new larger arrays */
        size_t new_cap   = (size_t)(n + 1);   /* indices 0 .. n   */
        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        seq_t *new_pow   = (seq_t *)malloc(new_cap * sizeof(seq_t));

        /* Copy old cached values */
        if (current_n > 0) {
            for (idx_t k = 0; k <= current_n; ++k)
                new_cache[k] = cache[k];
        }

        /* Update powers for existing k (1 .. current_n) */
        for (idx_t k = 1; k <= current_n; ++k) {
            /* new exponent = (current_n) -> (new n) : multiply by base 2*k */
            new_pow[k] = pow_arr[k] * (seq_t)(2 * k);
        }

        /* Compute powers for new k > current_n (i.e., k = current_n+1 .. n) */
        for (idx_t k = current_n + 1; k <= n; ++k) {
            /* (2*k)^(n-1)  (exponent grows with n) */
            new_pow[k] = pow_seq((seq_t)(2 * k), (int)(n - 1));
        }

        /* Unused entry for k=0 */
        new_pow[0] = 0;

        /* Compute a(n) and store it */
        seq_t sum = 0;
        for (idx_t k = 1; k <= n; ++k) {
            sum += new_pow[k] * binomial(n, k);
        }
        new_cache[n] = sum;

        /* Free old arrays */
        if (cache)   free(cache);
        if (pow_arr) free(pow_arr);

        /* Adopt new arrays */
        cache   = new_cache;
        pow_arr = new_pow;
        current_n = n;
    }

    /* Return memoised value */
    return cache[n];
}
