/*
 * A342468  Number of multiples of n up to n^2 containing the substring n in
 *          base 10.
 * https://oeis.org/A342468
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = |{k : 1 ≤ k ≤ n², n is a decimal substring of k}|
 * Range:     u8 n <= 19, u16 n <= 275, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

/* Convert a seq_t integer into its decimal digits (most‑significant first). */
static void digits_of_seq(seq_t x, uint8_t *buf, size_t *len) {
    size_t i = 0;
    if (x == 0) {
        buf[i++] = 0;
    } else {
        while (x != 0) {
            buf[i++] = (uint8_t)(x % 10);
            x = x / 10;
        }
        /* reverse to most‑significant first */
        for (size_t l = 0, r = i - 1; l < r; ++l, --r) {
            uint8_t t = buf[l];
            buf[l] = buf[r];
            buf[r] = t;
        }
    }
    *len = i;
}

seq_t a(idx_t n) {
    static uint8_t pat_buf[64];
    static uint8_t mult_buf[128];
    size_t pat_len;
    size_t mult_len;

    /* pattern for n */
    digits_of_seq((seq_t)n, pat_buf, &pat_len);

    seq_t count = 0;
    for (idx_t t = 1; t <= n; ++t) {
        seq_t m = (seq_t)n * (seq_t)t;
        digits_of_seq(m, mult_buf, &mult_len);
        if (mult_len < pat_len)
            continue;

        bool found = false;
        for (size_t i = 0; i + pat_len <= mult_len; ++i) {
            size_t k;
            for (k = 0; k < pat_len; ++k)
                if (mult_buf[i + k] != pat_buf[k])
                    break;
            if (k == pat_len) {
                found = true;
                break;
            }
        }
        if (found)
            count++;
    }
    return count;
}
