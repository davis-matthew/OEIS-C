/*
 * A363121  Primitive terms of A116882: terms k of A116882 such that k/2 is
 *          not a term of A116882.
 * https://oeis.org/A363121
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1 * a(n) = (2*n - 1) * 2^(floor_log2(n-1) + 2) for n > 1
 *            * where floor_log2(x) is ⌊log₂(x)⌋. * * This is equivalent to
 *            the OEIS formula a(n) = (2*n-1) * 2^A070941(n-1) * because for
 *            n-1 > 0, A070941(n-1) = ⌊log₂(n-1)⌋ + 2
 * Range:     u8 n <= 8, u16 n <= 128, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_uint.h"   /* seq_t pow_uint(uint64_t base, idx_t exp); */

/* Computes floor(log₂(x)) for a non‑zero 64‑bit integer x. */
static inline idx_t floor_log2_u64(idx_t x)
{
    /* For x ≠ 0, __builtin_clzll counts leading zeros. */
    return 63 - __builtin_clzll(x);
}

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    idx_t nm1 = n - 1;                     /* n-1 ≥ 1 here */
    idx_t log2 = floor_log2_u64(nm1);      /* ⌊log₂(n-1)⌋ */
    idx_t exp  = log2 + 2;                 /* exponent of 2 */

    seq_t pow2 = pow_uint(2, exp);          /* 2^exp  (big‑int aware) */
    seq_t term = (seq_t)(2 * n - 1) * pow2; /* (2n-1)*2^exp */

    return term;
}
