/*
 * A400395  a(n) = Sum_{d|n} n / gcud(d, n/d), where gcud is the greatest
 *          common unitary divisor.
 * https://oeis.org/A400395
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = Sum_{d|n} n / gcud(d, n/d) * * The function uses the
 *            multiplicative formula * a(p^e) = (e+1)*p^e if e is odd * a(p^e)
 *            = e*p^e + p^(e/2) if e is even. * * Sequences with idx_t =
 *            uint64_t and seq_t being any unsigned integer * type (uint8_t …
 *            uint64_t, unsigned __int128, or arbitrary‑precisi...
 * Range:     u8 n <= 39, u16 n <= 2159, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* local power helper using 128‑bit intermediate to avoid overflow
 * when seq_t is 64‑bit. The cast to seq_t is safe for values that fit. */
static seq_t pow_u128(uint64_t base, idx_t exp)
{
    unsigned __int128 r = 1;
    unsigned __int128 b = base;
    while (exp > 0) {
        r *= b;
        --exp;
    }
    return (seq_t)r;
}

seq_t a(idx_t n)
{
    if (n == 1ULL)
        return 1;

    seq_t result = 1;
    idx_t m = n;

    /* factorise n by trial division */
    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            idx_t e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }

            seq_t p_pow = pow_u128(p, e);
            seq_t term;
            if (e & 1U) {          /* odd exponent */
                term = (seq_t)(e + 1) * p_pow;
            } else {               /* even exponent */
                seq_t p_half_pow = pow_u128(p, e / 2);
                term = (seq_t)e * p_pow + p_half_pow;
            }
            result *= term;
        }
    }

    /* remaining prime factor > sqrt(n) */
    if (m > 1) {
        uint64_t p = m;           /* prime factor with exponent 1 */
        seq_t term = 2 * (seq_t)p;   /* (1+1)*p^1 */
        result *= term;
    }

    return result;
}
