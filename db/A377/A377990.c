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
 * Range:     u8 n <= 53, u16 n <= 5543, u32 n <= 8729, u64 n <= 8729,
 *            u128 n <= 8729, big any n
 * Helpers:   factor_u64, pow_uint, seq_mul, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factor_u64.h"
#include "../utils/pow_uint.h"
#include "../utils/sigma_of.h"
#include "../utils/seq_mul.h"

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
            result = seq_mul(result, pow_uint(pr[i], (idx_t)ex[i]));
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
            result = seq_mul(result, pow_uint(pr[i], (idx_t)ex[i]));
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
