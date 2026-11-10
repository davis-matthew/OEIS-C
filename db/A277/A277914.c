/*
 * A277914  a(n) is the number of zeros minus the number of ones among the
 *          first n terms of A076478.
 * https://oeis.org/A277914
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Sum_{i=0..n-1} (-1)^A076478(i); computed by skipping
 *            whole blocks (each of 2^k numbers contributing k bits, net 0)
 *            and summing popcounts for the partial block
 * Range:     u8 n <= 4351, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 97 terms through n = 9828 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A277914_calc(idx_t n)
{
    idx_t r = n;
    unsigned k = 1;
    for (;; ++k) {
        unsigned __int128 blk = ((unsigned __int128)1 << k) * k;
        if ((unsigned __int128)r < blk) break;
        r -= (idx_t)blk;
    }
    idx_t q = r / k;
    unsigned rem = (unsigned)(r % k);

    /* S = total popcount of 0..q-1 */
    idx_t S = 0;
    for (unsigned b = 0; b < 64; ++b) {
        if (b >= 1 && (q >> b) == 0 && b > 0) {
            /* higher bits contribute nothing beyond this point, except handled below */
        }
        idx_t full = (b + 1 < 64) ? (q >> (b + 1)) << b : 0;
        idx_t low = (b + 1 < 64) ? (q & (((idx_t)1 << (b + 1)) - 1)) : q;
        idx_t half = (idx_t)1 << b;
        S += full + (low > half ? low - half : 0);
    }

    idx_t zp = 0, op = 0;
    for (unsigned i = 0; i < rem; ++i) {
        if ((q >> (k - 1 - i)) & 1U) ++op; else ++zp;
    }

    seq_t pos = (seq_t)k * (seq_t)q + (seq_t)zp;
    seq_t neg = (seq_t)2 * (seq_t)S + (seq_t)op;
    return pos - neg;
}

seq_t a(idx_t n)
{
    return A277914_calc(n);
}
