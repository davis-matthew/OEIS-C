/*
 * A392142  Starting with a(1) = 1, each term is obtained by doubling the
 *          previous term and then deleting repeated digits, keeping only the
 *          first occurrence of each digit.
 * https://oeis.org/A392142
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = A137564(2*a(n-1)), a(1)=1, periodic with period 10 after
 *            n=21. A137564 keeps only the first occurrence of each decimal
 *            digit from left to right
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Validated: partial, pass, 60 terms through n = 60 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

/* Helper: remove repeated digits, keeping first occurrence. */
static seq_t dedup_digits(seq_t x)
{
    /* maximum decimal digits for 128-bit: 39, so 40 is enough for all built‐ins. */
    uint8_t digits[40];
    size_t dlen = 0;
    /* Extract digits least‑to‑most. */
    while (x > (seq_t)0) {
        uint8_t d = (uint8_t)(x % 10);
        digits[dlen++] = d;
        x /= 10;
    }
    if (dlen == 0) {   /* handle x == 0 if ever needed */
        digits[dlen++] = 0;
    }
    bool seen[10] = {false};
    seq_t result = 0;
    /* Process most‑to‑least to preserve order of first occurrences. */
    for (size_t i = dlen; i-- > 0;) {
        uint8_t d = digits[i];
        if (!seen[d]) {
            seen[d] = true;
            result = result * 10 + d;
        }
    }
    return result;
}

/* Sequence function. */
seq_t a(idx_t n)
{
    /* Cache of first 30 terms (up to the start of the period). */
    static seq_t cache[30] = {0};
    static bool init = false;

    if (!init) {
        /* Initialise first term. */
        cache[0] = 1;
        /* Compute up to term 30. */
        for (idx_t i = 2; i <= 30; ++i) {
            seq_t prev = cache[i - 2];          /* a(i-1) */
            seq_t doubled = prev * 2;           /* 2*a(i-1) */
            cache[i - 1] = dedup_digits(doubled); /* a(i) */
        }
        init = true;
    }

    if (n <= 30) {
        return cache[n - 1];
    }
    /* Period 10 starting at n = 21. Map any larger n into that period. */
    idx_t offset = (n - 21) % 10 + 21;          /* offset in [21,30] */
    return cache[offset - 1];
}
