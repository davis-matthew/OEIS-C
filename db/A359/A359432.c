/*
 * A359432  Dirichlet inverse of A327936, which is multiplicative sequence
 *          with a(p^e) = p if e >= p, otherwise 1.
 * https://oeis.org/A359432
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) = multiplicative function defined by a(1) = 1, and for a
 *            prime power p^e: a(p^e) = (1 - p)^(e/p) if p divides e, a(p^e) =
 *            -(1 - p)^((e-1)/p) if e ≡ 1 (mod p), a(p^e) = 0 otherwise. (Each
 *            factor is a power of the integer (1-p).)
 * Range:     i8 n <= 9968, i16 n <= 9968, i32 n <= 9968, i64 n <= 9968,
 *            i128 n <= 9968, big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

static inline seq_t factor_from_pe(uint64_t p, uint64_t e)
{
    seq_t base = (seq_t)1 - (seq_t)p;          /* 1 - p */
    if (e % p == 0) {
        seq_t exp = (seq_t)(e / p);
        return big_pow(base, exp);
    } else if (e % p == 1) {
        seq_t exp = (seq_t)((e - 1) / p);
        seq_t r = big_pow(base, exp);
        return -r;    /* unary minus works for seq_t */
    }
    return (seq_t)0;
}

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    seq_t result = (seq_t)1;
    uint64_t m = (uint64_t)n;
    uint64_t p;

    for (p = 2; p <= m / p; ++p) {
        if (m % p == 0) {
            uint64_t e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            seq_t fac = factor_from_pe(p, e);
            if (fac == (seq_t)0)
                return (seq_t)0;
            result *= fac;
        }
    }

    if (m > 1) {               /* remaining prime factor */
        seq_t fac = factor_from_pe(m, 1);
        if (fac == (seq_t)0)
            return (seq_t)0;
        result *= fac;
    }

    return result;
}
