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
 * Validated: standard, pass, 145 terms through n = 980 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n): For 1 < k <= n, find the largest m such that k^1,
   k^2, …, k^m are palindromes in base n.  a(n) is the smallest k
   attaining that maximal m.  (Offset 2) */
#include <stdint.h>
#include <stdbool.h>

/* Return true iff val expressed in base 'base' is a palindrome.
   Works for arbitrary 128‑bit unsigned values. */
static bool is_palindrome_uint128(idx_t base, unsigned __int128 val)
{
    if (val == 0) return true;
    unsigned digits[128];
    unsigned cnt = 0;
    unsigned __int128 b = (unsigned __int128)base;
    while (val) {
        unsigned __int128 q = val / b;
        unsigned r = (unsigned)(val - q * b);
        digits[cnt++] = r;
        val = q;
    }
    unsigned i = 0, j = cnt;
    while (i < j / 2) {
        if (digits[i] != digits[j - 1 - i]) return false;
        ++i;
    }
    return true;
}

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
