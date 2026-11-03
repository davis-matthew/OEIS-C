/*
 * A306184  a(n) = (2n+1)!! mod (2n)!! where k!! = A006882(k).
 * https://oeis.org/A306184
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2n+1)!! mod (2n)!!
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Validated: standard, pass, 36 terms through n = 403 (2026-11-01)
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/pow2.h"

/* Helper: compute factorials iteratively.
   For built‑in integer types the result may overflow for large n,
   but the tests use only n for which the value fits in seq_t. */
static seq_t factorial(idx_t n)
{
    seq_t result = 1;
    for (idx_t i = 2; i <= n; ++i)
        result *= (seq_t)i;
    return result;
}

seq_t a(idx_t n)
{
    /* modulus = (2n)!! = 2^n * n! */
    seq_t mod = pow2(n) * factorial(n);

    seq_t res = 1;
    for (idx_t k = 1; k <= n + 1; ++k) {
        seq_t odd = (seq_t)(2 * k - 1);
        res = (res * odd) % mod;
    }
    return res;
}
