/*
 * A370004  Least k>0 such that the decimal expansion of k^2 contains k+n as a
 *          substring.
 * https://oeis.org/A370004
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Least k>0 such that decimal expansion of k^2 contains k+n
 *            as a substring
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_len, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow10.h"
#include "../utils/digit_len.h"

seq_t a(idx_t n) {
    seq_t sn = (seq_t)n;

    idx_t k = 1;
    for (;;) {
        seq_t kSeq = (seq_t)k;
        seq_t k2   = kSeq * kSeq;
        seq_t s    = kSeq + sn;

        int lenS  = digit_len(s);
        seq_t powLen = pow10(lenS);
        int maxp  = digit_len(k2) - lenS;

        for (int p = 0; p <= maxp; ++p) {
            seq_t powp = pow10(p);
            seq_t sub  = (k2 / powp) % powLen;
            if (sub == s) {
                return kSeq;
            }
        }
        ++k;
    }
}
