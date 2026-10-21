/*
 * A258757  For 1 < k <= n, let m be the largest number such that k^1, k^2,
 *          ... k^m are palindromes in base n. a(n) gives the smallest k which
 *          has the largest value of m.
 * https://oeis.org/A258757
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_palindrome_uint128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n): For 1 < k <= n, find the largest m such that k^1,
   k^2, …, k^m are palindromes in base n.  a(n) is the smallest k
   attaining that maximal m.  (Offset 2) */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_palindrome_uint128.h"

seq_t a(idx_t n)
{
    if (n < 2) return 0;               /* out of offset, not used */
    seq_t best_k = 2;
    unsigned best_m = 0u;

    for (idx_t k = 2; k <= n; ++k) {
        unsigned m = 0;
        unsigned __int128 val = (unsigned __int128)k;
        while (1) {
            if (!is_palindrome_uint128(n, val))
                break;
            ++m;
            /* Prevent overflow: stop if next multiply would overflow */
            if (val > ((unsigned __int128)(~(unsigned __int128)0)) / k)
                break;
            val *= k;
        }
        if (m > best_m || (m == best_m && k < best_k)) {
            best_m = m;
            best_k = (seq_t)k;
        }
    }
    return best_k;
}
