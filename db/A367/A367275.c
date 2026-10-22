/*
 * A367275  a(n) = 3^(n - 1) * (9^n - 1) / 8, n >= 1.
 * https://oeis.org/A367275
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 14, u128 n <= 18,
 *            big any n
 * Helpers:   pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A367275: a(n) = 3^(n - 1) * (9^n - 1) / 8, n >= 1
 * The formula gives an integer because 9^n ≡ 1 (mod 8).
 * This implementation computes 3^(n-1) and 9^n in sequence
 * arithmetic, performs the division by 8 first to keep the
 * intermediate product within bounds for small seq_t types.
 */

#include "../utils/pow_int.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        /* Undefined for n = 0; return 0 to avoid UB. */
        return (seq_t)0;
    }

    /* Compute 3^(n-1). */
    seq_t pow3_n_minus1 = pow_int((seq_t)3, n - 1);

    /* Compute 9^n = 3^(2n). */
    idx_t two_n = n << 1;            /* 2 * n */
    seq_t pow9_n = pow_int((seq_t)3, two_n);

    /* (9^n - 1) / 8 – division is exact. */
    seq_t part = (pow9_n - (seq_t)1) / (seq_t)8;

    /* Final result: 3^(n-1) * part. */
    return pow3_n_minus1 * part;
}
