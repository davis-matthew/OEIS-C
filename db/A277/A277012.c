/*
 * A277012  Factorial base representation of n is rewritten as a base-2 number
 *          with each nonzero digit k replaced by a run of k 1's (followed by
 *          one extra zero if not the rightmost run of 1's) and with each 0
 *          kept as 0.
 * https://oeis.org/A277012
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = rewrite factorial base of n in binary as described: each
 *            non‑zero digit k becomes k consecutive 1‑bits followed by a
 *            separating 0 (unless it is the lowest non‑zero run); each 0
 *            digit stays a 0
 * Range:     u8 n <= 40, u16 n <= 478, u32 n <= 5040, u64 n <= 5040,
 *            u128 n <= 5040, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
seq_t a(idx_t n)
{
    /* Gather factorial base digits (least significant first). */
    int digits[64];
    int len = 0;
    idx_t tmp = n;
    while (tmp > 0) {
        idx_t base = (idx_t)(len + 2);          /* base = k+1 */
        digits[len++] = (int)(tmp % base);     /* digit < base ≤ 20 for 64‑bit n */
        tmp /= base;
    }

    /* Pre‑compute whether any lower digit (less significant) is non‑zero. */
    bool lower_nonzero[64];
    lower_nonzero[0] = false;
    for (int i = 1; i < len; ++i) {
        lower_nonzero[i] = lower_nonzero[i - 1] || (digits[i - 1] != 0);
    }

    /* Build the binary value from most significant digit to least. */
    seq_t res = 0;
    for (int i = len - 1; i >= 0; --i) {
        int d = digits[i];
        if (d == 0) {
            res <<= 1;                          /* a single 0 bit */
        } else {
            for (int b = 0; b < d; ++b) {
                res = (res << 1) | 1;           /* append d consecutive 1s */
            }
            if (lower_nonzero[i]) {             /* not the lowest non‑zero run */
                res <<= 1;                      /* separator zero */
            }
        }
    }
    return res;
}
