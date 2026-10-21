/*
 * A242474  Numbers n such that A = n - digitsum(n) is divisible by the
 *          largest power of 10 <= A.
 * https://oeis.org/A242474
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = n for 1 ≤ n ≤ 19 a(n) = 90*10^(⌊(n-20)/10⌋+1) + (10 +
 *            ((n-20) mod 10)) for n > 19 (OEIS definition: numbers n such
 *            that n - sumdigits(n) = 9*10^k, with all 1–19 included.)
 * Range:     u8 n <= 19, u16 n <= 39, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    if (n <= (idx_t)19) {
        return (seq_t)n;
    }
    idx_t offset = n - (idx_t)19;                 /* 1‑based offset after the first 19 terms */
    idx_t k = (offset - 1) / 10;                  /* block index (0 for the 910‑919 block) */
    seq_t pow = pow10(k + 1);                     /* 10^(k+1) */
    seq_t base = pow * (seq_t)90;                 /* 90 * 10^(k+1) */
    seq_t t = (seq_t)(10 + (offset - 1) % 10);    /* t = 10..19 within the block */
    return base + t;
}
