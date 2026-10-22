/*
 * A307991  Fibonacci numbers of the form k^2 - k - 1 with integer k.
 * https://oeis.org/A307991
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, bref, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A307991: Fibonacci numbers of the form k^2 - k - 1 (k ∈ ℕ) */
#include <stdbool.h>
#include <stddef.h>
#include "../utils/floor_sqrt.h"

/* Test whether a number is Fibonacci using the 5n²±4 perfect‑square test.  */
static bool is_fib(seq_t n)
{
    seq_t sq1 = 5 * n * n + 4;
    seq_t sqrt1 = floor_sqrt(sq1);
    if (sqrt1 * sqrt1 == sq1) return true;

    seq_t sq2 = 5 * n * n - 4;
    seq_t sqrt2 = floor_sqrt(sq2);
    return sqrt2 * sqrt2 == sq2;
}

/* Return the nth term (1‑based) of the sequence.  For n>4 the value is 0. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* the sequence is 1‑based */

    const seq_t SEQ_T_MAX = (seq_t)(~(seq_t)0);          /* max value representable by seq_t */
    const unsigned __int128 MAX128 = (unsigned __int128)SEQ_T_MAX;

    size_t found = 0;            /* number of terms found so far */
    idx_t k = 2;                 /* start testing from k = 2 */

    while (found < n) {
        unsigned __int128 tmp = (unsigned __int128)k * k - k - 1;
        if (tmp > MAX128) break;                       /* further values cannot fit */

        seq_t val = (seq_t)tmp;
        if (is_fib(val)) {
            found++;
            if (found == n) return val;                /* found the requested term */
        }
        k++;                                          /* try next k */
    }

    return 0; /* no further terms (sequence has only 4 non‑zero terms) */
}
