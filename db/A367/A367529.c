/*
 * A367529  The number of ways of tiling the n X n grid up to diagonal and
 *          antidiagonal reflections by a tile that is not fixed under any of
 *          these symmetries.
 * https://oeis.org/A367529
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(2m-1) = 256^(m^2 - m) a(2m) = 1/4 (16^(m^2) + 256^(m^2)) where
 *            m >= 1
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 7,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"   /* pow2(idx_t exp) -> seq_t 2^exp */

seq_t a(idx_t n) {
    /* ensure n >= 1 */
    if (n == 0) return (seq_t)0;  /* should not happen, but guard */
    /* determine odd or even case */
    if ((n & 1) == 1) {          /* odd n = 2m-1 */
        unsigned long long m = (n + 1) >> 1;          /* m = (n+1)/2 */
        unsigned long long m2 = m * m;               /* m^2 */
        unsigned long long shift = 8ULL * (m2 - m);   /* 8*(m^2 - m) */
        return pow2((idx_t)shift);                   /* 2^(8*(m^2-m)) = 256^(m^2-m) */
    } else {                      /* even n = 2m */
        unsigned long long m = n >> 1;               /* m = n/2 */
        unsigned long long m2 = m * m;               /* m^2 */
        unsigned long long shift1 = 4ULL * m2;       /* 4*m^2  = log2(16^m^2) */
        unsigned long long shift2 = 8ULL * m2;       /* 8*m^2  = log2(256^m^2) */

        seq_t term = pow2((idx_t)shift1) + pow2((idx_t)shift2);  /* 16^m2 + 256^m2 */
        return term >> 2;        /* divide by 4 (exact) */
    }
}
