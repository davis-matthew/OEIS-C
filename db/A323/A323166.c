/*
 * A323166  Greatest common divisor of Product (1+(p_i^e_i)) and n, when n =
 *          Product (p_i^e_i); a(n) = gcd(A034448(n), n).
 * https://oeis.org/A323166
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = gcd(n, ∏_{p^e‖n} (1 + p^e))
 * Range:     u8 n <= 815, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 121 terms through n = 9038 (2026-11-01)
 * Helpers:   gcd, ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The product is the sum of unitary divisors (A034448). */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/ipow_u64_idx.h"

seq_t a(idx_t n)
{
    if (n == 0)                /* undefined, but guard against underflow */
        return 0;

    seq_t unitary_sum = 1;     /* product over prime powers */
    idx_t m = n;               /* remaining part to factor */

    /* Trial division up to √m */
    for (idx_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            idx_t exponent = 0;
            while (m % p == 0) {
                m /= p;
                ++exponent;
            }
            /* Term = 1 + p^exponent */
            seq_t term = 1 + ipow_u64_idx((uint64_t)p, exponent);
            unitary_sum *= term;
        }
    }

    /* Remaining prime factor, if any */
    if (m > 1) {
        seq_t term = 1 + (seq_t)m;
        unitary_sum *= term;
    }

    /* a(n) = gcd(n, unitary_sum) */
    return gcd(unitary_sum, (seq_t)n);
}
