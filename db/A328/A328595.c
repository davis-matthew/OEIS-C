/*
 * A328595  Numbers whose reversed binary expansion is a necklace.
 * https://oeis.org/A328595
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   iterative search for numbers whose reversed binary expansion is
 *            a necklace. The function a(n) returns the nth term in ascending
 *            order
 * Range:     u8 n <= 85, u16 n <= 8907, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 77 terms through n = 9296 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

static inline bool is_necklace_rev_bin(idx_t x)
{
    /* Determine bit length */
    int len = 0;
    idx_t t = x;
    while (t > 0) {
        len++;
        t >>= 1;
    }
    if (len == 0) return false; /* unreachable for x>=1 */

    /* Build reversed binary string: s[0] is least significant bit of x */
    unsigned char s[64];           /* len <= 64 for idx_t == uint64_t */
    t = x;
    for (int i = 0; i < len; ++i) {
        s[i] = (unsigned char)(t & 1);
        t >>= 1;
    }

    /* Booth's algorithm to find least lexicographic rotation */
    int i = 0, j = 1, k = 0;
    while (i < len && j < len && k < len) {
        unsigned char a = s[(i + k) % len];
        unsigned char b = s[(j + k) % len];
        if (a == b) {
            ++k;
            continue;
        }
        if (a > b) {
            i = i + k + 1;
            if (i <= j) i = j + 1;
        } else { /* a < b */
            j = j + k + 1;
            if (j <= i) j = i + 1;
        }
        k = 0;
    }
    int start = (i < j) ? i : j;
    return start == 0;   /* true if original reversed string is minimal */
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* not used, but guard against underflow */
    idx_t count = 0;
    idx_t candidate = 1;
    while (1) {
        if (is_necklace_rev_bin(candidate)) {
            ++count;
            if (count == n) return (seq_t)candidate;
        }
        ++candidate;
    }
}
