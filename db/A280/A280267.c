/*
 * A280267  Square array read by rows: H_k(n,n), with rows n >= 0 and columns
 *          0 <= k <= 3, where H_n is the n-th hyperoperation.
 * https://oeis.org/A280267
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = H_{n mod 4}(floor(n/4), floor(n/4))
 * Range:     u8 n <= 18, u16 n <= 30, u32 n <= 42, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    idx_t row = n / 4;
    idx_t col = n & 3;             /* n % 4, but faster with bit‑mask */
    seq_t base = (seq_t)row;
    switch (col) {
        case 0:
            return base + 1;        /* H0(n, n) = n + 1 */
        case 1:
            return base + base;     /* H1(n, n) = 2n */
        case 2:
            return base * base;     /* H2(n, n) = n² */
        case 3:
            return big_pow(base, base);  /* H3(n, n) = nⁿ */
        default:
            return (seq_t)0; /* unreachable */
    }
}
