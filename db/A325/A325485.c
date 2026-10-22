/*
 * A325485  One of the four successive approximations up to 5^n for the 5-adic
 *          integer 6^(1/4). This is the 2 (mod 5) case (except for n = 0).
 * https://oeis.org/A325485
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0; for n > 0, a(n) is the unique k in [1,5^n] with * k ≡
 *            2 (mod 5) and k^4 ≡ 6 (mod 5^n). This implementation * computes
 *            a(n) by successive Hensel lifting using only integer *
 *            arithmetic
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mod_pow.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;

    seq_t k = 2;          /* current root modulo 5^m, starting at m=1 */
    seq_t pow5 = 5;       /* 5^m */
    for (idx_t m = 1; m < n; ++m) {
        seq_t prevPow = pow5;   /* 5^m */
        pow5 *= 5;              /* 5^(m+1) */

        /* Compute current root to 5^(m+1) */
        seq_t k4   = mod_pow(k, 4, pow5);
        seq_t diff = k4 - 6;                 /* >=0 */
        seq_t u    = diff / prevPow;         /* integer division gives coeff of 5^m */
        seq_t s    = u % 5;                  /* s = (k^4 - 6)/5^m  (mod 5) */

        /* Next digit t in base 5: t = (-s * 3) mod 5 */
        seq_t t = (5 - (s * 3) % 5) % 5;

        /* Update k: k' = k + t * 5^m */
        k += t * prevPow;
    }
    return k;
}
