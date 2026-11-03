/*
 * A265559  Smallest base-2 palindrome m >= n, written in base 2.
 * https://oeis.org/A265559
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 31, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Validated: partial, pass, 47 terms through n = 46 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the smallest base‑2 palindrome m ≥ n and returns its binary
 * representation interpreted as a decimal integer. The sequence is
 * a(n) = binary_to_decimal(m) where m is the least integer ≥ n whose
 * binary expansion is a palindrome. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
}

/* Test whether x's binary representation is a palindrome. */
static bool is_bin_pal(idx_t x)
{
    if (x <= 1u) return true;              /* 0 and 1 are palindromes */

    /* Determine bit length. */
    idx_t len = 0;
    idx_t y = x;
    while (y) { len++; y >>= 1; }

    /* Compare symmetric bits. */
    idx_t i = 0, j = len - 1;
    while (i < j) {
        if (((x >> i) & 1u) != ((x >> j) & 1u))
            return false;
        ++i; --j;
    }
    return true;
}

/* Convert the binary representation of m into a decimal integer. */
static seq_t bits_to_decimal(idx_t m)
{
    seq_t v = 0;

    /* Handle m == 0 specially (binary "0"). */
    if (m == 0) return 0;

    /* Compute bit length. */
    idx_t len = 0;
    idx_t t = m;
    while (t) { ++len; t >>= 1; }

    /* Accumulate decimal value from most significant bit. */
    for (idx_t i = len; i > 0; --i) {
        v = mul_small(v, 10);                 /* v = v * 10 */
        v += (seq_t)((m >> (i-1)) & 1u);      /* add current bit */
    }
    return v;
}

seq_t a(idx_t n)
{
    for (idx_t m = n; ; ++m) {
        if (is_bin_pal(m))
            return bits_to_decimal(m);
    }
}
