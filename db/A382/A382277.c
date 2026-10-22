/*
 * A382277  a(n) is the least composite number obtained by inserting a
 *          nonempty string of 0's inside n.
 * https://oeis.org/A382277
 *
 * Offset:    10    (a(10) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 8206, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the least composite number obtained by inserting a nonempty
 * string of 0's inside the decimal representation of the index n.
 * For each position between two consecutive digits of n, we try inserting
 * 1, 2, 3, ... zeros until we get a composite number, then keep the
 * smallest such composite among all positions.
 * n is assumed to be >= 10; behaviour for smaller n is undefined.
 */

#include <stdbool.h>
#include "../utils/pow10.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n) {
    /* If n is less than 10 we are outside the defined region; return 0.
     * The sequence is only needed for n >= 10. */
    if (n < 10) return 0;

    /* Extract decimal digits of n into an array, most‑significant first. */
    idx_t temp = n;
    idx_t digits[20];  /* Enough for 64‑bit integers (~20 digits). */
    idx_t k = 0;
    while (temp > 0) {
        digits[k++] = temp % 10;
        temp /= 10;
    }
    /* Reverse to get most‑significant first. */
    for (idx_t i = 0; i < k / 2; ++i) {
        idx_t tmp = digits[i];
        digits[i] = digits[k - 1 - i];
        digits[k - 1 - i] = tmp;
    }

    seq_t best = 0;  /* 0 indicates no candidate yet. */

    /* Iterate over all positions between digits. */
    for (idx_t pos = 0; pos + 1 < k; ++pos) {
        /* Try inserting t zeros, starting from 1. */
        for (idx_t t = 1; ; ++t) {
            seq_t val = 0;
            for (idx_t j = 0; j < k; ++j) {
                val = val * 10 + digits[j];
                if (j == pos) {
                    /* Insert t zeros after this digit. */
                    val *= pow10(t);
                }
            }
            /* Check if val is composite. */
            if (!is_prime_big(val)) {
                if (best == 0 || val < best) best = val;
                break;  /* Found the smallest composite for this position. */
            }
            /* For safety, stop if t becomes too large (should not happen). */
            if (t > 10) break;
        }
    }

    return best;
}
