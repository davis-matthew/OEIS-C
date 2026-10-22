/*
 * A400366  Number of Proth numbers <= n.
 * https://oeis.org/A400366
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = #{ (m, k) | m ≥ 1, k odd, 1 ≤ k < 2^m, k*2^m + 1 ≤ n }
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 2) return 0;

    seq_t count = 0;
    idx_t power = 2;           /* 2^1 */

    while (power + 1 <= n) {
        /* maximum k so that k*2^m+1 ≤ n  */
        idx_t max_k = (n - 1) / power;
        if (max_k > power - 1) max_k = power - 1;   /* enforce k < 2^m */
        /* count odd k ≤ max_k : (max_k + 1)/2 */
        count += ((seq_t)max_k + 1) / 2;

        /* prepare next power of 2, avoid overflow */
        if (power > (UINT64_MAX >> 1)) break;
        power <<= 1;
    }

    return count;
}
