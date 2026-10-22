/*
 * A333791  Difference of sums of two subsets of divisors of n, those obtained
 *          by repeatedly dividing with the smallest remaining prime factor
 *          (A332993) and those obtained by repeatedly dividing with the
 *          largest remaining prime factor (A332994).
 * https://oeis.org/A333791
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A332993(n) - A332994(n) (difference of sums obtained by
 *            repeatedly dividing by the smallest and the largest remaining
 *            prime factor)
 * Range:     u8 n <= 327, u16 n <= 8816, u32 n <= 8816, u64 n <= 8816,
 *            u128 n <= 8816, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Smallest and largest prime factor tables, rebuilt up to the
   requested limit when necessary. */
static uint32_t *spf = NULL;
static uint32_t *lpf = NULL;
static idx_t sieve_max = 0;

/* Rebuild the factor tables at least up to 'limit'. */
static void recompute_sieve(idx_t limit)
{
    if (limit <= sieve_max)
        return;

    free(spf);
    free(lpf);
    spf = (uint32_t*)calloc((size_t)(limit + 1), sizeof(uint32_t));
    lpf = (uint32_t*)calloc((size_t)(limit + 1), sizeof(uint32_t));
    if (!spf || !lpf)
        abort();

    for (idx_t i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {                 /* i is prime */
            spf[i] = (uint32_t)i;
            lpf[i] = (uint32_t)i;
            for (idx_t j = i; j <= limit; j += i) {
                if (spf[j] == 0)
                    spf[j] = (uint32_t)i; /* first hit = smallest prime factor */
                lpf[j] = (uint32_t)i;     /* later hit = largest prime factor */
            }
        }
    }
    sieve_max = limit;
}

/* Compute the n-th term of OEIS sequence A333791. */
seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;               /* a(1) = 0 */

    recompute_sieve(n);

    seq_t sum_small = (seq_t)n;
    idx_t  m = n;
    while (m > 1) {
        uint32_t p = spf[m];
        m = m / (idx_t)p;
        sum_small += (seq_t)m;
    }

    seq_t sum_large = (seq_t)n;
    m = n;
    while (m > 1) {
        uint32_t p = lpf[m];
        m = m / (idx_t)p;
        sum_large += (seq_t)m;
    }

    return sum_small - sum_large;
}
