/*
 * A257564  Irregular triangle read by rows: T(n,k) = r(n+k)+r(n-k) with r(n)
 *          = (n-(n mod 2))/2 for n>=0 and -n<=k<=n.
 * https://oeis.org/A257564
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabf, easy, nonn
 * Formula:   a(n) = ⌊(offset)/2⌋ + ⌊(2r - offset)/2⌋ where r = ⌊√n⌋ and
 *            offset = n - r²
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* integer square root for unsigned 64‑bit values, using only 64‑bit
   arithmetic (no floating point).  Returns ⌊√n⌋. */
static inline idx_t floor_sqrt_u64(idx_t n)
{
    idx_t res = 0;
    idx_t bit = 1ULL << 62;            /* 2⁶² is the highest power of four ≤ 2⁶⁴ */
    while (bit > n)
        bit >>= 2;                     /* reduce to the highest power of four ≤ n */
    while (bit != 0) {
        if (n >= res + bit) {
            n -= res + bit;
            res = (res >> 1) + bit;
        } else {
            res >>= 1;
        }
        bit >>= 2;
    }
    return res;
}

seq_t a(idx_t n)
{
    idx_t r      = floor_sqrt_u64(n);          /* r = ⌊√n⌋                        */
    idx_t offset = n - r * r;                  /* offset within the row (0..2r)   */

    /* T(r,k) = floor(offset/2) + floor((2r - offset)/2)  */
    seq_t part1 = (seq_t)(offset >> 1);
    seq_t part2 = (seq_t)(((2 * r) - offset) >> 1);
    return part1 + part2;
}
