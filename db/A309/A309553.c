/*
 * A309553  Imperfect numbers of the form 2^(2^k-1)*F_1*F_2*...*F_(k-1), where
 *          F is a Fermat number.
 * https://oeis.org/A309553
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = 2^(2^(n+1)-1) * ((2^(2^(n+1)) - 1) / 3) for 1 ≤ n ≤ 4;
 *            a(n)=0 otherwise
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;             /* offset is 1, so return 0 for non‑existent indices */
    idx_t k = n + 1;                  /* map n to k (k=2 for a(1), k=3 for a(2), …) */
    if (k > 5) return 0;              /* only k = 2..5 produce imperfect numbers */
    idx_t exp1 = (1ULL << k) - 1;     /* exponent 2^k - 1 */
    seq_t pow2_exp = pow_uint(2, exp1);          /* 2^(2^k-1) */
    seq_t pow2_full = pow_uint(2, 1ULL << k);    /* 2^(2^k)  */
    seq_t prod = (pow2_full - 1) / 3;            /* product of F1 .. F_{k-1} */
    return pow2_exp * prod;
}
