/*
 * A340313  The n-th squarefree number is the a(n)-th squarefree number having
 *          its number of primes.
 * https://oeis.org/A340313
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{x<=n : A072047(x) = A072047(n)}; computed by sieving
 *            omega and squarefree flags, then counting omega classes among
 *            the first n squarefree numbers
 * Range:     u8 n <= 539, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 88 terms through n = 9973 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;

    size_t bound = (size_t)n * 2 + 100;
    uint8_t *om = (uint8_t *)calloc(bound + 1, 1);
    uint8_t *nsf = (uint8_t *)calloc(bound + 1, 1); /* 1 = not squarefree */
    if (!om || !nsf) {
        free(om);
        free(nsf);
        return (seq_t)0;
    }

    for (size_t p = 2; p <= bound; ++p) {
        if (om[p] == 0) { /* p is prime */
            for (size_t j = p; j <= bound; j += p)
                om[j]++;
            if (p <= bound / p) {
                size_t pp = p * p;
                for (size_t j = pp; j <= bound; j += pp)
                    nsf[j] = 1;
            }
        }
    }

    uint64_t cnt[64];
    for (int i = 0; i < 64; ++i)
        cnt[i] = 0;

    idx_t found = 0;
    seq_t result = 0;
    for (size_t i = 1; i <= bound; ++i) {
        if (!nsf[i]) {
            ++found;
            cnt[om[i]]++;
            if (found == n) {
                result = (seq_t)cnt[om[i]];
                break;
            }
        }
    }

    free(om);
    free(nsf);
    return result;
}
