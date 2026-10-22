/*
 * A391713  Number of divisors d | n such that d is neither squarefree nor
 *          powerful (in A332785).
 * https://oeis.org/A391713
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A000005(n) - A005361(n) - 2^A001221(n) + 1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) counts the divisors d|n that are neither square‑free nor powerful. */

#include "../utils/pow2.h"

seq_t a(idx_t n) {
    /* n >= 1 by OEIS offset; we handle all n >= 1. */
    idx_t tmp = n;
    seq_t d = 1;              /* number of divisors, A000005(n) */
    seq_t powerful = 1;       /* number of powerful divisors, A005361(n) */
    idx_t distinct = 0;       /* ω(n), number of distinct primes */

    for (idx_t p = 2; p <= tmp / p; ++p) {
        if (tmp % p == 0) {
            ++distinct;
            idx_t exp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++exp;
            }
            d *= (seq_t)(exp + 1);
            if (exp >= 2)
                powerful *= (seq_t)exp;
        }
    }

    if (tmp > 1) {          /* remaining prime factor of exponent 1 */
        ++distinct;
        d *= 2;              /* factor (exp + 1) where exp = 1 */
        /* powerful remains unchanged because exp < 2 */
    }

    seq_t squarefree = pow2(distinct); /* 2^ω(n) */
    seq_t result = d - powerful - squarefree + 1;
    return result;
}
