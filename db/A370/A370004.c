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
 * Validated: standard, pass, 87 terms through n = 9731 (2026-11-01)
 * Helpers:   num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/num_digits.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

seq_t a(idx_t n) {
    seq_t sn = (seq_t)n;

    idx_t k = 1;
    for (;;) {
        seq_t kSeq = (seq_t)k;
        seq_t k2   = kSeq * kSeq;
        seq_t s    = kSeq + sn;

        int lenS  = num_digits(s);
        seq_t powLen = pow10(lenS);
        int maxp  = num_digits(k2) - lenS;

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
