/*
 * A377990  a(n) = sigma(sigma(A350388(n))) * sigma(sigma(A350389(n))), where
 *          A350388 and A350389 are the largest unitary divisor of n that is a
 *          square, and the largest unitary divisor of n that is an
 *          exponentially odd number, respectively.
 * https://oeis.org/A377990
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sigma(sigma(A350388(n))) * sigma(sigma(A350389(n)))
 * Range:     u8 n <= 53, u16 n <= 5543, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 87 terms through n = 8729 (2026-11-01)
 * Helpers:   factor_u64, ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/factor_u64.h"
#include "../utils/ipow_u64_idx.h"

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

static seq_t unitary_square_divisor(uint64_t n)
{
    /* largest unitary divisor of n that is a perfect square */
    /* product of all p^e with even exponent e */
    const size_t MAX_FACTORS = 64; /* plenty for any 64‑bit n  */
    uint64_t pr[MAX_FACTORS], ex[MAX_FACTORS];
    size_t cnt = 0;
    factor_u64(n, pr, ex, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i)
        if ((ex[i] & 1u) == 0)           /* exponent even */
            result = seq_mul(result, ipow_u64_idx(pr[i], (idx_t)ex[i]));
    return result;
}

static seq_t unitary_odd_divisor(uint64_t n)
{
    /* largest unitary divisor of n that is exponentially odd */
    /* product of all p^e with odd exponent e */
    const size_t MAX_FACTORS = 64;
    uint64_t pr[MAX_FACTORS], ex[MAX_FACTORS];
    size_t cnt = 0;
    factor_u64(n, pr, ex, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i)
        if (ex[i] & 1u)                  /* exponent odd */
            result = seq_mul(result, ipow_u64_idx(pr[i], (idx_t)ex[i]));
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* safety; a(1) is first term */

    uint64_t un = (uint64_t)n;

    /* compute the two unitary divisors */
    seq_t d1 = unitary_square_divisor(un);   /* A350388(n) */
    seq_t d2 = unitary_odd_divisor(un);     /* A350389(n) */

    /* first sigma */
    seq_t s1 = sigma_of((idx_t)d1);
    seq_t s2 = sigma_of((idx_t)d2);

    /* sigma of the sigma */
    seq_t ss1 = sigma_of((idx_t)s1);
    seq_t ss2 = sigma_of((idx_t)s2);

    /* final product */
    return seq_mul(ss1, ss2);
}
