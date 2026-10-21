/*
 * A252493  Numbers n such that n(n+1) is 13-smooth. (Related to the abc
 *          conjecture.)
 * https://oeis.org/A252493
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   is_13_smooth
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers n such that n*(n+1) is 13‑smooth.
   The sequence is finite (68 terms) and ends at 123200. */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_13_smooth.h"

/* Local static storage for the sequence.  128 slots are plenty
   for the 68 terms that exist. */
static seq_t values[128];
static idx_t count = 0;  /* how many terms have been stored */

/* Build the entire sequence (once).  The proven upper bound for the largest
   term is 123200, so we examine all n up to that number. */
static void build_sequence(void)
{
    /* Allocate space (already statically allocated above). */
    count = 0;
    for (idx_t i = 1; i <= 123200; ++i) {
        seq_t ni = (seq_t)i;
        if (is_13_smooth(ni) && is_13_smooth(ni + 1)) {
            values[count++] = ni;
        }
    }
}

/* External‑linkage function required by the framework. */
seq_t a(idx_t n)
{
    /* Lazily compute the sequence on first use. */
    static bool computed = false;
    if (!computed) {
        build_sequence();
        computed = true;
    }

    /* The sequence is finite; if n is out of bounds return 0. */
    if (n == 0 || n > count)
        return (seq_t)0;
    return values[n - 1];
}
