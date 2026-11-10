/*
 * A350235  a(n) is the least k such that A350226(k) = n.
 * https://oeis.org/A350235
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k such that A350226(k) = n. Computed by a search:
 *            for each step d with (n-1)*d < best, check every residue class
 *            mod d for a run of n Thue-Morse-constant terms. A failed check
 *            skips ahead past the mismatch. The first best comes from
 *            s_2(j*(2^m-1)) = m for 1 <= j <= 2^m, which gives a...
 * Range:     u8 n <= 18, u16 n <= 258, u32 n <= 4099, u64 n <= 4099,
 *            u128 n <= 4099, big any n
 * Validated: standard, pass, 67 terms through n = 4082 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static inline unsigned A350235_tm(idx_t x)
{
    uint64_t v = (uint64_t)x;
    v ^= v >> 32; v ^= v >> 16; v ^= v >> 8; v ^= v >> 4; v ^= v >> 2; v ^= v >> 1;
    return (unsigned)(v & 1U);
}

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)0;

    /* Upper bound from difference 2^m - 1 with 2^m >= n */
    idx_t p = 2;
    while (p < n) p *= 2;
    idx_t best = n * (p - 1);

    idx_t nm1 = n - 1;
    for (idx_t d = 1; nm1 * d < best; ++d)
    {
        for (idx_t r = 0; r < d; ++r)
        {
            idx_t j = nm1;               /* candidate end index within class */
            while (1)
            {
                idx_t x = r + j * d;
                if (x >= best) break;
                unsigned t = A350235_tm(x);
                idx_t len = 1;
                while (len <= j && len < n && A350235_tm(x - len * d) == t) ++len;
                if (len >= n) { best = x; break; }
                /* run ending at index j has length len < n; the next possible
                   end of a length-n run is (j - len) + n */
                j = j - len + n;
            }
        }
    }
    return (seq_t)best;
}
