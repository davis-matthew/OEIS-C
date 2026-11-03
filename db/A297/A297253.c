/*
 * A297253  Numbers whose base-4 digits having equal up-variation and
 *          down-variation; see Comments.
 * https://oeis.org/A297253
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 74 terms through n = 9391 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS A297253 by brute‑force search over natural
   numbers, checking for equal up‑variation and down‑variation in base‑4. */

#include <stdbool.h>
#include <stddef.h>

/* Helper: true if a number's base‑4 digits have equal up‑variation and
   down‑variation. */
static bool equal_variation(seq_t x)
{
    seq_t dv = 0;                 /* down‑variation: sum of positive rises   */
    seq_t uv = 0;                 /* up‑variation:   sum of positive falls   */
    seq_t t = x;
    bool first = true;
    int prev = 0;

    /* Scan digits from least significant to most (order does not affect
       equality of up and down variations). */
    while (t > 0)
    {
        int digit = (int)(t % 4);
        if (first)
        {
            prev = digit;
            first = false;
        }
        else
        {
            int diff = digit - prev;
            if (diff > 0)
                dv += diff;
            else if (diff < 0)
                uv -= diff;
            prev = digit;
        }
        t /= 4;
    }

    /* For a one‑digit number dv == uv == 0, which is acceptable. */
    return dv == uv;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* n is 1‑based. */
    seq_t target = (seq_t)n;   /* cast once for comparison */
    seq_t count  = 0;
    seq_t value  = 1;          /* start from the smallest positive integer */

    while (true)
    {
        if (equal_variation(value))
        {
            count++;
            if (count == target)
                return value;
        }
        value++;
    }
}
