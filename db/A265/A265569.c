/*
 * A265569  Smallest base-7 palindrome m >= n, written in base 7.
 * https://oeis.org/A265569
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = smallest base‑7 palindrome m >= n, written in base‑7
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   gen_half
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/gen_half.h"

seq_t a(idx_t n)
{
    seq_t target_n = (seq_t)n;

    /* Single‑digit palindromes cover 0 … 6 directly */
    if (n <= 6)
        return target_n;

    /* Generate palindromes of increasing length until one meets the criterion */
    for (int L = 2; ; ++L) {
        int halfLen = (L % 2 == 0) ? (L / 2) : ((L + 1) / 2);
        int digits[64];
        seq_t out;
        if (gen_half(L, halfLen, 0, digits, target_n, &out))
            return out;
    }
}
