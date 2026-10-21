/*
 * A297283  Numbers whose base-14 digits have equal down-variation and
 *          up-variation; see Comments.
 * https://oeis.org/A297283
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 4694, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers whose base‑14 first and last digits are equal. */
/* This function returns the n‑th such number (n ≥ 1). */
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* single‑digit numbers */
    if (n <= 13)
        return (seq_t)n;

    /* determine the base‑14 length of the n‑th number */
    idx_t cumulative = 13;      /* numbers with length 1 */
    idx_t len = 2;              /* current length being examined */
    idx_t pow = 1;              /* 14^(len‑2) */

    while (cumulative + 13 * pow < n) {
        cumulative += 13 * pow;
        ++len;
        pow *= 14;
    }

    /* offset of the desired number within this length (0‑based) */
    idx_t offset      = n - cumulative - 1;
    idx_t first_digit = offset / pow + 1;    /* most significant digit (1–13) */
    idx_t remainder   = offset % pow;        /* encodes the middle digits */

    /* middle digits: len‑2 digits, each 0–13, padded with leading zeros */
    const size_t MAXLEN = 32;   /* ample for n ≤ 10000 */
    uint8_t middle[MAXLEN];
    size_t midlen = (size_t)(len - 2);

    for (size_t i = 0; i < midlen; ++i) {
        middle[midlen - 1 - i] = remainder % 14;
        remainder /= 14;
    }

    /* convert base‑14 digits to decimal (seq_t) */
    seq_t val = (seq_t)first_digit;   /* most significant digit */
    for (size_t i = 0; i < midlen; ++i)
        val = val * 14 + (seq_t)middle[i];
    val = val * 14 + (seq_t)first_digit;   /* least significant digit */

    return val;
}
