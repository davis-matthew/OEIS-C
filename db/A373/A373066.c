/*
 * A373066  Numbers k such that the Hankel matrix formed using the digits of k
 *          is singular.
 * https://oeis.org/A373066
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 438, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   hankel_singular
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A373066 – Numbers k such that the Hankel matrix
   formed from the decimal digits of k is singular.
   Computation is performed by enumerating integers in order and
   testing singularity via the Bareiss algorithm on the Hankel matrix. */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/hankel_singular.h"

/* Sequence cache – a static array of maximum size needed for the tests
   (enough for n up to about 10000). */
enum { MAX_CAP = 200000 };
static seq_t values[MAX_CAP];
static size_t val_len = 0;
static idx_t next_candidate = 0;

/* Return the n‑th term of the sequence (1‑based). */
seq_t a(idx_t n)
{
    /* Guard against too large n – the array has been chosen large
       enough for the expected limits. */
    if (n == 0)
        return 0;        /* should never happen for the OEIS offset */

    while (val_len < (size_t)n) {
        if (hankel_singular(next_candidate)) {
            if (val_len < MAX_CAP)
                values[val_len++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }
    return values[(size_t)n - 1];
}
