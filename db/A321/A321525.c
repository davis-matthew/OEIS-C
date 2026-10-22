/*
 * A321525  Left-rotate nonzero digits in ternary expansion of n and convert
 *          back to decimal.
 * https://oeis.org/A321525
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = left‑rotate the non‑zero digits of n's ternary
 *            representation and convert back to decimal
 * Range:     u8 n <= 244, u16 n <= 6561, u32 n <= 6561, u64 n <= 6561,
 *            u128 n <= 6561, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Computes a(n) for the sequence A321525.
   Uses only the integer types seq_t and idx_t provided by the build. */
seq_t a(idx_t n) {
    /* Special case */
    if (n == 0)
        return (seq_t)0;

    /* Temporary storage for ternary digits (most significant first).
       n < 2^64, so at most 40 ternary digits.  A fixed 64‑byte buffer
       is ample for all builds, including 128‑bit seq_t. */
    uint8_t digits[64];
    size_t len = 0;
    idx_t tmp = n;

    /* Extract digits least‑significant first. */
    while (tmp > 0) {
        digits[len++] = (uint8_t)(tmp % 3);
        tmp /= 3;
    }

    /* Reverse to most‑significant first. */
    for (size_t i = 0; i < len / 2; ++i) {
        uint8_t t = digits[i];
        digits[i] = digits[len - 1 - i];
        digits[len - 1 - i] = t;
    }

    /* Positions of non‑zero digits and their values. */
    uint8_t pos[64];
    uint8_t nz[64];
    size_t nz_cnt = 0;
    for (size_t i = 0; i < len; ++i) {
        if (digits[i] != 0) {
            pos[nz_cnt] = (uint8_t)i;
            nz[nz_cnt] = digits[i];
            ++nz_cnt;
        }
    }

    /* Left‑rotate the non‑zero digits if there are two or more. */
    if (nz_cnt > 1) {
        uint8_t first = nz[0];
        for (size_t i = 0; i < nz_cnt - 1; ++i)
            nz[i] = nz[i + 1];
        nz[nz_cnt - 1] = first;
    }

    /* Put rotated digits back into their original positions. */
    for (size_t i = 0; i < nz_cnt; ++i)
        digits[pos[i]] = nz[i];

    /* Reconstruct the integer from the modified ternary digits. */
    seq_t result = (seq_t)0;
    for (size_t i = 0; i < len; ++i)
        result = result * 3 + (seq_t)digits[i];

    return result;
}
