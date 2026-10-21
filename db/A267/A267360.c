/*
 * A267360  Decimal representation of the n-th iteration of the "Rule 125"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A267360
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A267360
 * Decimal representation of the n-th iteration of Rule 125.
 * Recurrence (valid for n > 8):
 *   a(n) = 17*a(n-2) - 16*a(n-4)
 * Initial values:
 *   a(0)=1, a(1)=3, a(2)=28, a(3)=47,
 *   a(4)=496, a(5)=575, a(6)=8000, a(7)=9215, a(8)=128000
 *
 * The code below implements this recurrence directly.
 */

#include <stddef.h>   /* for size_t */
#include <stdlib.h>   /* for malloc, free */

/* The build supplies the following typedefs:
 *   typedef <unsigned integer type> seq_t;   // e.g. uint64_t or __int128
 *   typedef <unsigned integer type> idx_t;   // e.g. uint64_t
 * The function has external linkage.
 */
seq_t a(idx_t n)
{
    /* Base values for indices 0..8 */
    static const seq_t init_vals[9] = {1, 3, 28, 47, 496, 575, 8000, 9215, 128000};

    /* For n <= 8, return the pre‑computed value */
    if (n <= 8) {
        return init_vals[n];
    }

    /* Allocate an array large enough to hold values up to n
     * (including the base values).  An idx_t may contain > 32 bits,
     *  so we cast to size_t for allocation when it is safe. */
    size_t len = (size_t)(n + 1);
    seq_t *buf = (seq_t *)malloc(len * sizeof(seq_t));
    if (!buf) {
        /* Allocation failed; aborting is acceptable as the caller
         * is expected to handle this in a larger context. */
        abort();
    }

    /* Initialize base cases */
    size_t i;
    for (i = 0; i < 9; ++i) {
        buf[i] = init_vals[i];
    }

    /* Compute subsequent terms using the recurrence:
     * a(i) = 17*a(i-2) - 16*a(i-4)   for i >= 9
     */
    for (i = 9; i <= n; ++i) {
        buf[i] = (seq_t)17 * buf[i - 2] - (seq_t)16 * buf[i - 4];
    }

    /* Extract result and free temporary storage */
    seq_t result = buf[n];
    free(buf);
    return result;
}
