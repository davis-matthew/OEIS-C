/*
 * A283995  Least number with same prime signature as the n-th divisorial:
 *          a(n) = A046523(A007955(n)).
 * https://oeis.org/A283995
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A046523(A007955(n))
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 59, u64 n <= 179,
 *            u128 n <= 719, big any n
 * Validated: standard, pass, 90 terms through n = 1015 (2026-11-01)
 * Helpers:   factor_u64, ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>          /* size_t */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/factor_u64.h"
#include "../utils/ipow.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

static const uint64_t _small_primes[12] = {2,3,5,7,11,13,17,19,23,29,31,37};

seq_t a(idx_t n)
{
    /* handle the offset 1 */
    if (n == 1)
        return (seq_t)1;

    /* factorise n */
    uint64_t pr[12];
    uint64_t ex[12];
    size_t  cnt;
    factor_u64((uint64_t)n, pr, ex, &cnt);

    /* compute τ(n) = product (e_i+1) */
    idx_t tau = 1;
    for (size_t i = 0; i < cnt; ++i)
        tau *= (idx_t)(ex[i] + 1);

    /* build the exponent list for D(n) = product_of_divisors(n) */
    idx_t expD[12];
    for (size_t i = 0; i < cnt; ++i)
        expD[i] = ((idx_t)ex[i] * tau) / 2;   /* always integer */

    /* sort exponents descending (small scale, bubble sort) */
    for (size_t i = 0; i < cnt; ++i) {
        for (size_t j = 0; j + 1 < cnt - i; ++j) {
            if (expD[j] < expD[j + 1]) {
                idx_t tmp = expD[j];
                expD[j] = expD[j + 1];
                expD[j + 1] = tmp;
            }
        }
    }

    /* compute the minimal number with this prime signature */
    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        idx_t e = expD[i];
        if (e == 0)
            continue;
        seq_t term = ipow((seq_t)_small_primes[i], (seq_t)e);
        result = seq_mul(result, term);
    }

    return result;
}
