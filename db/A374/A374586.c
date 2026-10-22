/*
 * A374586  The maximum exponent in the prime factorization of the numbers
 *          that are divisible by the maximum exponent in their prime
 *          factorization.
 * https://oeis.org/A374586
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the maximum exponent in the prime factorization of the
 *            n-th integer k (k ≥ 2) such that k is divisible by that exponent
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* Upper limit for the sieve – large enough to cover the first 10 000 terms.
   The density of qualifying numbers is >0.9, so 2 million covers all 10 000
   terms with ample safety. */
#define SPF_LIMIT 2000000

/* Smallest prime factor table – 0 indicates that the entry is unused. */
static uint32_t spf[SPF_LIMIT + 1] = {0};
static bool spf_ready = false;

/* Initialise the SPF array once. */
static void init_spf(void)
{
    for (idx_t i = 0; i <= SPF_LIMIT; ++i)
        spf[i] = 0;

    for (idx_t i = 2; i <= SPF_LIMIT; ++i) {
        if (spf[i] == 0) {
            spf[i] = (uint32_t)i;
            if (i <= SPF_LIMIT / i) {
                for (idx_t j = i * i; j <= SPF_LIMIT; j += i) {
                    if (spf[j] == 0)
                        spf[j] = (uint32_t)i;
                }
            }
        }
    }
    spf_ready = true;
}

seq_t a(idx_t n)
{
    if (!spf_ready)
        init_spf();

    idx_t count = 0;
    for (idx_t k = 2; k <= SPF_LIMIT; ++k) {
        idx_t temp = k;
        idx_t emax = 0;

        /* compute maximum exponent in the prime factorisation of k */
        while (temp > 1) {
            idx_t p = spf[temp];
            idx_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            if (e > emax)
                emax = e;
        }

        /* check divisibility condition */
        if (k % emax == 0) {
            ++count;
            if (count == n)
                return (seq_t)emax;
        }
    }

    /* If n is larger than the number of qualifying integers below SPF_LIMIT,
       the sequence is undefined for the requested index. Return 0 as a
       placeholder; this situation should not occur for the intended input
       sizes (up to n ≈ 10 000). */
    return (seq_t)0;
}
