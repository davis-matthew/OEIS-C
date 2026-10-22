/*
 * A327193  For any n >= 0: consider the different ways to split the binary
 *          representation of n into two (possibly empty) parts, say with
 *          value x and y; a(n) is the greatest possible value of min(x, y).
 * https://oeis.org/A327193
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look, base
 * Formula:   a(n) = max over all binary splits of min(prefix, suffix) of n
 * Range:     u8 n <= 861, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   floor_log2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_log2_uint64.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    unsigned len = floor_log2_uint64(n) + 1;   /* number of bits of n */
    seq_t best = 0;
    for (unsigned k = 0; k <= len; ++k) {
        seq_t pref, suff;
        if (k == 0) {
            pref = 0;
            suff = (seq_t)n;
        } else if (k == len) {
            pref = (seq_t)n;
            suff = 0;
        } else {
            unsigned shift = len - k;          /* 1 <= shift <= len-1 <= 63 */
            pref = ((seq_t)n) >> shift;
            suff = ((seq_t)n) & (((seq_t)1 << shift) - 1);
        }
        seq_t cur = pref < suff ? pref : suff;
        if (cur > best) best = cur;
    }
    return best;
}
