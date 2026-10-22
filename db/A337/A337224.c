/*
 * A337224  a(n) is the least number that can be obtained by replacing some
 *          repetitive part X^k in the binary expansion of n by X.
 * https://oeis.org/A337224
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1093, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A337224: minimal integer obtained by replacing a repeated binary substring X^k with X. */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Convert n to binary bit array (MSB first). */
    unsigned char bits[65];
    idx_t L = 0;

    if (n == 0) {
        bits[0] = 0;
        L = 1;
    } else {
        idx_t pos = 0;
        idx_t temp = n;
        while (temp != 0) {
            bits[pos++] = (unsigned char)(temp & 1);
            temp >>= 1;
        }
        /* Reverse to get most‑significant bit first. */
        for (idx_t i = 0; i < pos / 2; ++i) {
            unsigned char t = bits[i];
            bits[i] = bits[pos - 1 - i];
            bits[pos - 1 - i] = t;
        }
        L = pos;
    }

    /* Start with the original number as the best candidate. */
    seq_t best = (seq_t)n;

    /* Enumerate all possible repeated substrings X^k. */
    for (idx_t i = 0; i < L; ++i) {
        for (idx_t p = 1; p * 2 <= L - i; ++p) {
            idx_t maxK = (L - i) / p;
            bool ok = true;

            for (idx_t k = 2; k <= maxK; ++k) {
                /* Verify that the k‑th block matches the first block. */
                for (idx_t j = 0; j < p; ++j) {
                    if (bits[i + (k - 1) * p + j] != bits[i + j]) {
                        ok = false;
                        break;
                    }
                }
                if (!ok) break;   /* Larger k will also fail. */

                /* Compute the value after replacing X^k with X. */
                seq_t val = 0;
                for (idx_t idx = 0; idx < i; ++idx) {
                    val = (val << 1) + (seq_t)bits[idx];
                }
                for (idx_t idx = i; idx < i + p; ++idx) {
                    val = (val << 1) + (seq_t)bits[idx];
                }
                for (idx_t idx = i + k * p; idx < L; ++idx) {
                    val = (val << 1) + (seq_t)bits[idx];
                }
                if (val < best) best = val;
            }
        }
    }

    return best;
}
