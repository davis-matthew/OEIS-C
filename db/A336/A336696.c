/*
 * A336696  Sum of odd divisors of 1+sigma(n).
 * https://oeis.org/A336696
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of odd divisors of 1 + sigma(n)
 * Range:     u8 n <= 83, u16 n <= 9914, u32 n <= 9914, u64 n <= 9914,
 *            u128 n <= 9914, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is defined for n >= 1.  sigma(n) is the sum of
   the positive divisors of n.  This function keeps a cache of all
   sigma values seen so far and expands it when required.  When
   computing the sum of odd divisors of (1 + sigma(n)) we iterate
   over all divisor pairs (d, e) with d <= sqrt(x), adding d or e
   only if the value is odd.  This correctly handles the case
   where an even divisor pairs with an odd divisor. */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Guard against n == 0, though the offset is 1. */
    if (n == 0) return 0;

    /* Cache of sigma values up to the largest n seen so far. */
    static seq_t *sigma = NULL;
    static idx_t sigma_size = 0;

    /* Resize and recompute sigma[] if a larger n is requested. */
    if (n > sigma_size) {
        idx_t new_size = n + 1;  /* space for indices 0..n */
        seq_t *new_sigma = (seq_t *)malloc((size_t)new_size * sizeof(seq_t));
        if (!new_sigma) return 0;  /* OOM guard */

        /* Initialise to zero. */
        for (idx_t i = 0; i < new_size; ++i) {
            new_sigma[i] = 0;
        }

        /* Sieve the sum-of-divisors function. */
        for (idx_t i = 1; i <= n; ++i) {
            seq_t ii = (seq_t)i;
            for (idx_t j = i; j <= n; j += i) {
                new_sigma[j] += ii;
            }
        }

        /* Replace old cache. */
        if (sigma) free(sigma);
        sigma = new_sigma;
        sigma_size = n;
    }

    /* Compute x = 1 + sigma(n). */
    seq_t x = sigma[n] + (seq_t)1;

    /* Sum odd divisors of x. */
    seq_t sum = 0;
    for (idx_t d = 1; (seq_t)d * (seq_t)d <= x; ++d) {
        if (x % (seq_t)d == 0) {
            seq_t d_val = (seq_t)d;
            seq_t e = x / d_val;  /* complementary divisor */

            if (d_val & 1) sum += d_val;          /* add d if odd */
            if (e != d_val && (e & 1)) sum += e;  /* add e if odd and distinct */
        }
    }

    return sum;
}
