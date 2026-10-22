/*
 * A334387  The difference version of the 'Decade transform' : to obtain a(n)
 *          write n as a sum of its power-of-ten parts and then continue to
 *          calculate the absolute value of the difference between the
 *          adjacent parts until a single number remains.
 * https://oeis.org/A334387
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   digits, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Decade-difference transform:
 *   For a non‑negative integer n, write it in decimal and split it into its
 *   power‑of‑ten parts:
 *          n = Σ d_i·10^i   (0 ≤ d_i ≤ 9)
 *   Then repeatedly replace the list of parts by the absolute
 *   differences of adjacent parts until only one part remains.
 *   That final part is a(n).  For single‑digit n the value is n itself.
 *
 *   The implementation uses only integer arithmetic and works for any
 *   seq_t and idx_t supplied by the build environment.
 */

#include "../utils/pow10.h"
#include "../utils/digits.h"

static inline seq_t absdiff(seq_t a, seq_t b)
{
    return a >= b ? a - b : b - a;
}

seq_t a(idx_t n)
{
    /* Determine number of decimal digits (at least 1 for n = 0). */
    int d = digits(n);
    /* Temporary array of individual decimal digits, most significant first. */
    int dig[64];                       /* 64 digits safely covers any 64‑bit index */
    idx_t tmp = n;
    for (int i = d - 1; i >= 0; --i) { /* fill from least significant to most */
        dig[i] = tmp % 10;
        tmp /= 10;
    }

    /* Build the list of power‑of‑ten parts. */
    seq_t parts[64];
    seq_t newparts[64];
    for (int i = 0; i < d; ++i) {
        /* dig[i] is the i‑th digit from most significant side.
         * Its power is 10^(d-1-i). */
        parts[i] = (seq_t)dig[i] * pow10((idx_t)(d - 1 - i));
    }

    int len = d;
    while (len > 1) {
        /* Compute absolute differences of adjacent parts. */
        for (int j = 0; j < len - 1; ++j) {
            newparts[j] = absdiff(parts[j], parts[j + 1]);
        }
        --len;
        /* Copy results back into parts array for next iteration. */
        for (int j = 0; j < len; ++j) {
            parts[j] = newparts[j];
        }
    }

    return parts[0];
}
