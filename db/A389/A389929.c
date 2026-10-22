/*
 * A389929  Smallest prime number obtained by removing or adding a single
 *          digit anywhere in n, or 0 if no such prime exists.
 * https://oeis.org/A389929
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = smallest prime obtainable by removing or adding a single
 *            digit in the decimal representation of n, returning 0 if no such
 *            prime exists
 * Range:     u8 n <= 13, u16 n <= 4764, u32 n <= 9999, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

#define MAX_DIG 64  /* enough for indices up to 10^18 and beyond */

seq_t a(idx_t n)
{
    /* Convert n to its decimal digits (most significant first). */
    uint8_t digits[MAX_DIG];
    int dlen = 0;
    idx_t temp = n;

    if (temp == 0) {
        digits[0] = 0;
        dlen = 1;
    } else {
        while (temp > 0 && dlen < MAX_DIG) {
            digits[dlen++] = temp % 10;
            temp /= 10;
        }
        /* reverse to get most‑significant first */
        for (int i = 0; i < dlen / 2; ++i) {
            uint8_t t = digits[i];
            digits[i] = digits[dlen - 1 - i];
            digits[dlen - 1 - i] = t;
        }
    }

    seq_t best = 0;   /* 0 means no prime found yet */

    /* 1. Try removing each digit */
    for (int i = 0; i < dlen; ++i) {
        seq_t val = 0;
        for (int j = 0; j < dlen; ++j) {
            if (j == i) continue;
            val = val * 10 + digits[j];
        }
        if (val != 0 && is_prime_big(val)) {
            if (best == 0 || val < best) best = val;
        }
    }

    /* 2. Try inserting a digit (0–9) at every position (0…dlen) */
    for (int pos = 0; pos <= dlen; ++pos) {
        for (int d = 0; d <= 9; ++d) {
            seq_t val = 0;
            for (int j = 0; j < pos; ++j) val = val * 10 + digits[j];
            val = val * 10 + d;
            for (int j = pos; j < dlen; ++j) val = val * 10 + digits[j];
            if (val != 0 && is_prime_big(val)) {
                if (best == 0 || val < best) best = val;
            }
        }
    }

    return best;
}
