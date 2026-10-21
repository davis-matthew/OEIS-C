/*
 * A277698  a(n) = least unitary prime divisor of n or 1 if no such
 *          prime-divisor exists.
 * https://oeis.org/A277698
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least unitary prime divisor of n or 1 if no such divisor
 *            exists
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* a(n) returns the least prime divisor of n that does not occur with exponent ≥ 2.
   If n = 1 or all prime divisors occur at least twice, 1 is returned. */
seq_t a(idx_t n) {
    if (n == 0) return 1;               /* safety for out-of-range calls */
    if (n == 1) return 1;                /* by definition */
    uint64_t m = (uint64_t)n;           /* local copy for manipulation */

    /* Handle smallest possible divisor 2 separately for speed. */
    if (m % 2 == 0) {
        uint64_t exp = 0;
        while (m % 2 == 0) {
            m /= 2;
            ++exp;
        }
        if (exp == 1)            /* 2 appears only once */
            return (seq_t)2;
        /* otherwise 2 was squared or higher, continue searching */
    }

    /* Trial division by odd numbers up to sqrt(m). */
    for (uint64_t d = 3; d * d <= m; d += 2) {
        if (m % d == 0) {
            uint64_t exp = 0;
            while (m % d == 0) {
                m /= d;
                ++exp;
            }
            if (exp == 1)            /* d appears only once */
                return (seq_t)d;
            /* otherwise skip d */
        }
    }

    /* If a remainder > 1, it is a prime divisor with exponent 1. */
    if (m > 1)
        return (seq_t)m;

    /* No unitary prime divisor found (all exponents ≥ 2). */
    return (seq_t)1;
}
