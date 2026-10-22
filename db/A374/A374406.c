/*
 * A374406  Number of ways to write n as an ordered sum of a triangular
 *          number, a square and a hexagonal number.
 * https://oeis.org/A374406
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |{(t,s,h) : t = k(k+1)/2, s = m², h = p(2p-1), t+s+h =
 *            n}|
 * Range:     u8 n <= 7, u16 n <= 1965, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/floor_sqrt.h"   /* provides seq_t floor_sqrt(seq_t) */

/* Checks whether x is a hexagonal number (including 0). */
static inline bool is_hexagonal_seq(idx_t x)
{
    seq_t xs = (seq_t)x;
    if (xs == 0)
        return true;                       /* 0 = 0·(2·0−1) */

    /* 8x + 1 must be a perfect square with square root ≡ 3 (mod 4). */
    seq_t d   = 8 * xs + 1;
    seq_t r   = floor_sqrt(d);
    if (r * r != d)
        return false;

    /* Convert r to a native integer; r < 2^34 for all idx_t inputs. */
    uint64_t rw = (uint64_t)r;
    if ((rw & 3u) != 3u)
        return false;

    seq_t m = (r + 1) >> 2;              /* m = (r+1)/4 */
    return m * (2 * m - 1) == xs;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    seq_t nseq = (seq_t)n;
    seq_t count = 0;

    for (idx_t t = 0; ; ++t) {
        seq_t tri = (seq_t)t * (t + 1) / 2;
        if (tri > nseq)
            break;

        for (idx_t s = 0; ; ++s) {
            seq_t sq = (seq_t)s * s;
            if (tri + sq > nseq)
                break;

            seq_t rem = nseq - tri - sq;
            if (is_hexagonal_seq((idx_t)rem))
                ++count;
        }
    }

    return count;
}
