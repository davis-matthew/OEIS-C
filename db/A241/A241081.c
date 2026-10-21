/*
 * A241081  Numbers that are not perfect squares but that can never be ruled
 *          out as perfect squares by any final group of digits.
 * https://oeis.org/A241081
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A241081: numbers that are not perfect squares but can never be ruled out
   as perfect squares by any final group of digits.  A term is a positive integer
   that can be written as (40k+1 or 40k+9) × 4^i × 25^j with k,i,j ≥ 0, and
   where the factor 40k+1 or 40k+9 itself is NOT a perfect square.
   The implementation tests each candidate in increasing order, checking that
   the exponents of 2 and 5 are even, that the remaining odd part is congruent
   to 1 or 9 modulo 40, and that this odd part is not a perfect square. */

#include <stdbool.h>

static bool is_square_idx(idx_t x)
{
    idx_t low = 0, high = x;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        if (mid == 0) {
            if (x == 0) return true;
            low = mid + 1;
            continue;
        }
        if (mid > x / mid) /* mid^2 would overflow or be too large */
            high = mid - 1;
        else {
            idx_t sq = mid * mid;
            if (sq == x) return true;
            if (sq < x)
                low = mid + 1;
            else
                high = mid - 1;
        }
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined for offset 1, return 0 */
    idx_t count = 0;
    idx_t candidate = 1;
    while (count < n) {
        idx_t x = candidate;
        unsigned v2 = 0;
        while ((x & 1) == 0) {       /* count exponent of 2 */
            x >>= 1;
            v2++;
        }
        if ((v2 & 1) == 0) {         /* exponent of 2 is even */
            unsigned v5 = 0;
            while (x % 5 == 0) {     /* count exponent of 5 */
                x /= 5;
                v5++;
            }
            if ((v5 & 1) == 0) {     /* exponent of 5 is even */
                if ((x % 40 == 1) || (x % 40 == 9)) {
                    if (!is_square_idx(x)) {   /* odd part not a perfect square */
                        count++;
                        if (count == n)
                            return (seq_t)candidate;
                    }
                }
            }
        }
        candidate++;
    }
    return (seq_t)candidate;   /* never reached */
}
