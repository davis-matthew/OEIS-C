/*
 * A365221  Each term is a "Go flat integer" (GFI), but a(n) + a(n+1) is
 *          always a "Go down integer" (GDI). More details in the Comments
 *          section.
 * https://oeis.org/A365221
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) is the n‑th unused Go‑flat integer such that its sum with
 *            the * preceding term is a Go‑down integer
 * Range:     u8 n <= 5, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* Maximum number of terms we will generate (enough for n up to 10000).  */
#define MAX_TERMS 10000

/* Static storage for the sequence.  */
static seq_t seq[MAX_TERMS];
static size_t seq_len = 0;

/* Test whether an integer is a Go‑flat integer (first digit == last digit). */
static bool is_gfi(seq_t x)
{
    if (x == (seq_t)0)   /* 0 is allowed as a Go‑flat integer */
        return true;
    seq_t last = x % (seq_t)10;
    seq_t first = x;
    while (first >= (seq_t)10)
        first /= (seq_t)10;
    return first == last;
}

/* Test whether an integer is a Go‑down integer (at least two digits and first digit > last digit). */
static bool is_gdi(seq_t x)
{
    if (x < (seq_t)10)   /* need at least two digits */
        return false;
    seq_t last = x % (seq_t)10;
    seq_t first = x;
    while (first >= (seq_t)10)
        first /= (seq_t)10;
    return first > last;
}

/* Return the n‑th term of the sequence (n ≥ 1).  Behaviour for n < 1 is undefined. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* out of bounds – return 0 */

    /* Generate terms until we have at least n of them. */
    while (seq_len < (size_t)n)
    {
        /* First term is 1. */
        if (seq_len == 0)
        {
            seq[0] = (seq_t)1;
            seq_len = 1;
            continue;
        }

        seq_t current = seq[seq_len - 1];

        /* Search for the smallest unused Go‑flat integer that makes
           current + candidate a Go‑down integer.  */
        for (seq_t cand = 0; ; ++cand)
        {
            if (!is_gfi(cand))
                continue;

            /* Check that cand has not appeared previously. */
            bool unique = true;
            for (size_t i = 0; i < seq_len; ++i)
            {
                if (seq[i] == cand)
                {
                    unique = false;
                    break;
                }
            }
            if (!unique)
                continue;

            seq_t sum = current + cand;
            if (is_gdi(sum))
            {
                seq[seq_len++] = cand;
                break;
            }
        }
    }

    return seq[n - 1];
}
