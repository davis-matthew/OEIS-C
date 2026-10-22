/*
 * A334227  Length of the shortest prefix of the Thue-Morse sequence (A010060)
 *          containing, as subwords, all length-n blocks appearing in A010060.
 * https://oeis.org/A334227
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 0, a(1) = 2, a(2) = 7. For n >= 3: let i = ⌊log2(n-2)⌋,
 *            pow2 = 2^i, j = n - pow2. Then a(n) = 7*pow2 + j - 1
 * Range:     u8 n <= 55, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)7;

    idx_t m = n - 2;                         /* m >= 1 */
    /* Compute floor(log2(m)) using GCC/Clang builtin */
    idx_t i = 63 - __builtin_clzll(m);       /* 0 <= i <= 63 */
    idx_t pow2 = 1ULL << i;                  /* 2^i */
    idx_t j = n - pow2;                      /* 2 <= j <= 2^i+1 */

    seq_t res = (seq_t)7 * (seq_t)pow2 + (seq_t)j - 1;
    return res;
}
