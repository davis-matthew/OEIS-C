/*
 * A329200  The ghost iteration (A): add or subtract the number formed by
 *          absolute differences of digits (A040115), according to parity
 *          (even or odd).
 * https://oeis.org/A329200
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = n + (-1)^d * d, where d = A040115(n) – number formed by
 *            absolute differences of consecutive decimal digits of n. For
 *            single‑digit n, d = 0
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Validated: partial, pass, 76 terms through n = 75 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Helper: compute A040115(n) as a seq_t. */
static seq_t _a040115(idx_t n)
{
    if (n < 10)        /* single‑digit numbers → d = 0 */
        return (seq_t)0;

    /* Extract decimal digits, least significant first. */
    uint8_t digits[20];   /* 64‑bit max -> 20 decimal digits */
    size_t len = 0;
    while (n > 0)
    {
        digits[len++] = (uint8_t)(n % 10);
        n /= 10;
    }

    seq_t d = 0;
    /* Process from most significant to least significant digit. */
    for (size_t i = len - 1; i > 0; --i)
    {
        uint8_t a = digits[i];
        uint8_t b = digits[i - 1];
        uint8_t diff = (a > b) ? (a - b) : (b - a);
        d = d * (seq_t)10 + (seq_t)diff;
    }
    return d;
}

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    seq_t d = _a040115(n);

    /* If d is even, add; if odd, subtract. */
    if ((d & (seq_t)1) == 0)
        return N + d;
    else
        return N - d;
}
