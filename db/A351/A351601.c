/*
 * A351601  a(n) = n^3 * Sum_{d^2|n} 1 / d^3.
 * https://oeis.org/A351601
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = n^3 * Sum_{d^2|n} 1 / d^3 This implementation uses the
 *            multiplicative formula a(p^e) = p^3 * (p^(3e) - p^(3·⌊(e-1)/2⌋))
 *            / (p^3 - 1) (p prime)
 * Range:     u8 n <= 6, u16 n <= 39, u32 n <= 1547, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* integer power: base^exp, exp ≥ 0 */
static inline seq_t pow_small(seq_t base, idx_t exp)
{
    seq_t r = 1;
    for (idx_t i = 0; i < exp; ++i) r *= base;
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* undefined for n < 1 */
    if (n == 1) return 1;      /* a(1) = 1 */

    seq_t result = 1;
    uint64_t m = (uint64_t)n;  /* copy for factorisation */

    for (uint64_t p = 2; p <= m / p; ++p) {
        if (m % p == 0) {
            int e = 0;
            while (m % p == 0) { m /= p; ++e; }

            seq_t pseq = (seq_t)p;
            seq_t p3    = pseq * pseq * pseq;          /* p^3 */

            seq_t p_pow3e   = pow_small(pseq, 3u * (idx_t)e);
            idx_t exp_floor = 3u * ((e - 1) / 2);       /* 3*floor((e-1)/2) */
            seq_t p_pow_floor = pow_small(pseq, exp_floor);

            seq_t numerator = p_pow3e - p_pow_floor;   /* p^(3e) - p^(3·⌊(e-1)/2⌋) */
            seq_t denom     = p3 - 1;                  /* p^3 - 1 */

            seq_t temp  = numerator / denom;  /* exact division */
            seq_t term  = p3 * temp;          /* p^3 * temp */

            result *= term;
        }
    }

    /* Remaining prime factor > sqrt(original n), if any */
    if (m > 1) {
        uint64_t p = m;
        int e = 1;                      /* exponent of the remaining prime */

        seq_t pseq = (seq_t)p;
        seq_t p3    = pseq * pseq * pseq;

        seq_t p_pow3e   = pow_small(pseq, 3u);
        seq_t p_pow_floor = pow_small(pseq, 0u);  /* = 1 */

        seq_t numerator = p_pow3e - p_pow_floor;
        seq_t denom     = p3 - 1;

        seq_t temp  = numerator / denom;
        seq_t term  = p3 * temp;

        result *= term;
    }

    return result;
}
