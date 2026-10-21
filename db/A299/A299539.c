/*
 * A299539  Numbers n = d_1 d_2 ... d_k (in base 10) such that d_i + d_{k+1-i}
 *          = 10 for i = 1..k.
 * https://oeis.org/A299539
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = enumerate base‑10 numbers whose digits satisfy d[i] +
 *            d[k+1-i] = 10. The sequence is produced in increasing order by
 *            length and by lexicographic order of the left half digits
 * Range:     u8 n <= 11, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* Compute 9^m as a seq_t value. */
static seq_t pow9(unsigned m)
{
    seq_t r = 1;
    for (unsigned i = 0; i < m; ++i)
        r = r * (seq_t)9;
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0)            /* safety for wrong offset calls */
        return (seq_t)0;

    const seq_t target = (seq_t)n;
    seq_t cumulative   = 0;
    seq_t prev_cumulative = 0;
    idx_t k = 1;

    /* Find the length k that contains the nth term. */
    for (;; ++k) {
        unsigned m = (unsigned)(k / 2);
        seq_t count = pow9(m);
        prev_cumulative = cumulative;
        cumulative += count;
        if (cumulative >= target)
            break;
    }

    /* offset within the block of numbers of length k. */
    seq_t offset = target - prev_cumulative - (seq_t)1;
    unsigned m = (unsigned)(k / 2);

    /* Allocate storage for the left half digits (values 1–9). */
    uint8_t *digits = (m > 0) ? (uint8_t*)malloc(m) : NULL;

    /* Extract the digits in base‑9 (0–8) and map to 1–9. */
    seq_t div = pow9(m);
    for (unsigned i = 0; i < m; ++i) {
        div = div / (seq_t)9;                 /* div = 9^(m‑1‑i) */
        seq_t d = offset / div;
        offset -= d * div;
        digits[i] = (uint8_t)(d + (seq_t)1);  /* 1…9 */
    }

    /* Construct the number. */
    seq_t result = 0;
    for (unsigned i = 0; i < m; ++i)
        result = result * (seq_t)10 + (seq_t)digits[i];

    if (k % 2 == 1)                    /* middle digit for odd length */
        result = result * (seq_t)10 + (seq_t)5;

    for (int i = (int)m - 1; i >= 0; --i)
        result = result * (seq_t)10 + (seq_t)(10 - digits[i]);

    free(digits);
    return result;
}
