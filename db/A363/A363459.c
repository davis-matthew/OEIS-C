/*
 * A363459  Sum of the first n prime powers A246655.
 * https://oeis.org/A363459
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of the first n prime powers (p^e)
 * Range:     u8 n <= 17, u16 n <= 174, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* seq_t and idx_t are supplied by the build system. */
static bool is_prime_power(idx_t x, uint32_t *spf)
{
    uint32_t p = spf[x];
    idx_t y = x;
    while (y % p == 0) y /= p;
    return y == 1;
}

static void build_spf(idx_t limit, uint32_t *spf)
{
    for (idx_t i = 2; i * i <= limit; ++i) {
        if (spf[i] == 0) {
            spf[i] = (uint32_t)i;
            for (idx_t j = i * i; j <= limit; j += i) {
                if (spf[j] == 0) spf[j] = (uint32_t)i;
            }
        }
    }
    for (idx_t i = 2; i <= limit; ++i) {
        if (spf[i] == 0) spf[i] = (uint32_t)i;  /* prime > sqrt(limit) */
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;  /* offset 1 – undefined below */

    /* Rough upper bound for the nth prime power.  20 is sufficient for n≤10000
       and scales linearly for larger n.  If the estimate is too low we
       double the limit until we find enough terms. */
    idx_t limit = n < 10 ? 100 : n * 20;

    for (;;) {
        uint32_t *spf = (uint32_t*)calloc((size_t)(limit + 1), sizeof(uint32_t));
        if (!spf) abort();  /* out of memory */

        build_spf(limit, spf);

        seq_t sum = 0;
        idx_t count = 0;
        for (idx_t x = 2; x <= limit; ++x) {
            if (is_prime_power(x, spf)) {
                ++count;
                sum += (seq_t)x;
                if (count == n) {
                    free(spf);
                    return sum;
                }
            }
        }

        free(spf);
        limit *= 2;   /* increase the search range and retry */
    }
}
