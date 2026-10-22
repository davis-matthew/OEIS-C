/*
 * A332249  a(n) is the X-coordinate of the n-th point of the quadratic Koch
 *          curve. Sequence A332250 gives Y-coordinates.
 * https://oeis.org/A332249
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6310, u16 n <= 9746, u32 n <= 9746, u64 n <= 9746,
 *            u128 n <= 9746, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A332249: a(n) = real part of f(n)
 * where f(0)=0 and f(n+1)=f(n)+i^t(n).
 * t(n) = (#1's in base‑5 rep of n) – (#3's in base‑5 rep of n).
 * The contribution δ(n) equals  1  if t(n)≡0 (mod 4),
 *                                 –1  if t(n)≡2 (mod 4),
 *                                 0  otherwise.
 * Thus a(0)=0 and  a(n)=∑_{k=0}^{n-1}δ(k).                                 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* a(0) is 0 without entering the loop */
    if (n == 0) {
        return 0;
    }

    seq_t res = 0;                 /* cumulative sum of δ(k)     */
    int    count1 = 0;             /* number of 1's in current n  */
    int    count3 = 0;             /* number of 3's in current n  */
    uint8_t digits[64] = {0};      /* base‑5 digits, LSB first   */

    /* iterate over k = 0 … n‑1 */
    for (idx_t k = 0; k < n; ++k) {
        /* compute δ(k) from current count1,count3 */
        int t = count1 - count3;
        int tmod4 = ((t % 4) + 4) % 4;
        if (tmod4 == 0) {
            res += 1;
        } else if (tmod4 == 2) {
            res -= 1;
        }

        /* increment k to k+1 in base‑5, updating counts */
        for (size_t pos = 0; ; ++pos) {
            uint8_t old_val = digits[pos];
            uint8_t new_val = old_val + 1;
            digits[pos] = new_val;

            /* update counts for the digit that changed */
            if (old_val == 1)      count1--;
            else if (old_val == 3) count3--;
            if (new_val == 1)      count1++;
            else if (new_val == 3) count3++;

            if (new_val < 5) {           /* no carry, done */
                break;
            }
            /* carry: set current digit to 0 (not counted) and
               continue to more significant digit */
            digits[pos] = 0;
            /* old_val was 4, which had no count effects */
        }
    }
    return res;
}
