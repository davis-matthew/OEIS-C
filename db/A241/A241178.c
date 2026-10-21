/*
 * A241178  Numbers n such that there are exactly three numbers m with m +
 *          (some digit of m) = n.
 * https://oeis.org/A241178
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 72, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A241178
   Numbers n such that exactly three digits d exist with
   n - d containing digit d.  Computed by brute‑force search. */

#include <stddef.h>
#include <stdbool.h>

/* Utilities from the build.  The header provides a routine that
   extracts the decimal digits of a seq_t value into a buffer. */
#include "../utils/get_digits.h"

/* Check whether the decimal representation of x contains digit d (0‑9). */
static bool contains_digit(seq_t x, int d)
{
    uint8_t buf[256];
    size_t len = 0;
    get_digits(x, buf, &len);
    for (size_t i = 0; i < len; ++i)
        if (buf[i] == (uint8_t)d)
            return true;
    return false;
}

/* Return the n‑th term of the sequence (a(1) = 14). */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* guard against misuse; offset starts at 1 */

    idx_t found = 0;          /* how many terms found so far */
    seq_t cand = 2;          /* start testing from 2 upwards */

    /* Brute‑force search until the n‑th term is found. */
    while (found < n) {
        int cnt = 0;          /* number of valid digits for this candidate */

        for (int d = 0; d <= 9; ++d) {
            if (cand <= (seq_t)d)   /* would give non‑positive m */
                continue;

            seq_t m = cand - (seq_t)d;
            if (contains_digit(m, d))
                ++cnt;
            if (cnt > 3)
                break;         /* no need to count further */
        }

        if (cnt == 3) {       /* candidate is a sequence term */
            ++found;
            if (found == n)
                return cand;
        }

        ++cand;
    }

    /* unreachable */
    return (seq_t)0;
}
