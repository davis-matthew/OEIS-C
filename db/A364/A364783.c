/*
 * A364783  Order of the symplectic group of 6 X 6 matrices over Z_n.
 * https://oeis.org/A364783
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 8, u128 n <= 68,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS: A364783
 * Formula: a(n) = Π_{p|n} p^(21*v_p(n) - 12) * (p^2 - 1) * (p^4 - 1) * (p^6 - 1)
 * (where v_p(n) is the exponent of p in the factorisation of n)
 *
 * This implementation uses trial division to factor n and the utility
 * seq_pow for exponentiation.  It is valid C11 and C++17.
 */

#include <stdint.h>            /* for uint64_t */
#include "../utils/seq_pow.h"  /* seq_pow(seq_t, idx_t) */

/* Compute a(n) for n >= 1 */
seq_t a(idx_t n)
{
    if (n == 0)            /* safety: OEIS offset starts at 1 */
        return 0;

    seq_t result = 1;
    uint64_t m = (uint64_t)n;   /* use native integer for factorisation */

    /* special treatment for factor 2 */
    if (m % 2 == 0) {
        uint64_t k = 0;
        while (m % 2 == 0) {
            m /= 2;
            ++k;
        }
        seq_t p = (seq_t)2;
        seq_t part = seq_pow(p, 21 * k - 12);
        seq_t p2 = p * p - 1;
        seq_t p4 = seq_pow(p, 4) - 1;
        seq_t p6 = seq_pow(p, 6) - 1;
        result *= part;
        result *= p2;
        result *= p4;
        result *= p6;
    }

    /* trial division for odd primes */
    for (uint64_t d = 3; d * d <= m; d += 2) {
        if (m % d == 0) {
            uint64_t k = 0;
            while (m % d == 0) {
                m /= d;
                ++k;
            }
            seq_t p = (seq_t)d;
            seq_t part = seq_pow(p, 21 * k - 12);
            seq_t p2 = p * p - 1;
            seq_t p4 = seq_pow(p, 4) - 1;
            seq_t p6 = seq_pow(p, 6) - 1;
            result *= part;
            result *= p2;
            result *= p4;
            result *= p6;
        }
    }

    /* If a prime factor larger than sqrt(m) remains */
    if (m > 1) {
        seq_t p = (seq_t)m;     /* remaining prime (k = 1) */
        seq_t part = seq_pow(p, 21 - 12); /* 21*1 - 12 = 9 */
        seq_t p2 = p * p - 1;
        seq_t p4 = seq_pow(p, 4) - 1;
        seq_t p6 = seq_pow(p,
                            6) - 1;
        result *= part;
        result *= p2;
        result *= p4;
        result *= p6;
    }

    return result;
}
