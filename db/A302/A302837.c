/*
 * A302837  a(n) = [x^n] Product_{k>=1} 1/(1 - n*x^prime(k)).
 * https://oeis.org/A302837
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in ∏_{p prime} 1/(1 - n*x^p)
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 15, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* dp[s] = sum over partitions of s into primes of n^(parts) */
    size_t sz = (size_t)n + 1;
    seq_t *dp = (seq_t *)calloc(sz, sizeof(seq_t));
    if (!dp) return 0;          /* allocation failure */

    dp[0] = 1;                  /* empty partition */

    /* sieve primes up to n */
    uint8_t *comp = NULL;
    if (n >= 2) {
        comp = (uint8_t *)calloc(sz, sizeof(uint8_t));
    }

    if (comp) {
        idx_t limit = n;
        /* mark composites */
        for (idx_t p = 2; p <= limit / p; ++p) {
            if (!comp[p]) {
                for (idx_t m = p * p; m <= limit; m += p) {
                    comp[m] = 1;
                }
            }
        }

        seq_t base = (seq_t)n;   /* n as seq_t for multiplication */
        for (idx_t p = 2; p <= limit; ++p) {
            if (!comp[p]) {        /* p is prime */
                for (idx_t s = p; s <= limit; ++s) {
                    /* dp[s] += dp[s-p] * n */
                    dp[s] += dp[s - p] * base;
                }
            }
        }
        free(comp);
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
