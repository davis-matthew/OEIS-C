/*
 * A350289  Infinite binary Walsh matrix read by antidiagonals.
 * https://oeis.org/A350289
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = parity(popcount(i & j)) where (i, j) are the coordinates
 *            reached by reading the infinite binary Walsh matrix by
 *            antidiagonals. The diagonal index s is s = ⌊ (√(8*n + 1) – 1) /
 *            2 ⌋ The position inside that diagonal is t = n – s*(s+1)/2 then
 *            i = t , j = s – t a(n) = (popcount(i & j)) mod 2
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root of an unsigned __int128, result fits in idx_t. */
static idx_t _isqrt_u128(unsigned __int128 x)
{
    idx_t r = 0;
    idx_t bit = (idx_t)1 << 63;      /* highest bit of a 64‑bit value */
    while (bit != 0) {
        idx_t next = r | bit;
        if ((unsigned __int128)next * next <= x)
            r = next;
        bit >>= 1;
    }
    return r;
}

seq_t a(idx_t n)
{
    /* compute s = floor((sqrt(8*n + 1) - 1)/2) */
    unsigned __int128 val = (unsigned __int128)n * 8 + 1;
    idx_t sqrt_val = _isqrt_u128(val);
    idx_t s = (sqrt_val - 1) / 2;

    /* number of entries before current diagonal */
    unsigned __int128 tri_prev = (unsigned __int128)s * (s + 1) / 2;

    /* position inside the diagonal */
    idx_t t = n - (idx_t)tri_prev;
    idx_t i = t;
    idx_t j = s - t;

    /* parity of the popcount of i & j */
    idx_t parity = __builtin_popcountll((unsigned long long)(i & j)) & 1;
    return (seq_t)parity;
}
