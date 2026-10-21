/*
 * A264551  Number of (2+1) X (n+1) arrays of permutations of 0..n*3+2 with
 *          each element having directed index change -1,1 -1,2 1,0 or 0,-1.
 * https://oeis.org/A264551
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1, a(2) = 2, a(3) = 6, a(4) = 16 Recurrence for n >= 5:
 *            a(n) = 2*a(n-1) + 8*a(n-4)
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 25, u64 n <= 49, u128 n <= 97,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* seq_t and idx_t are defined by the build; do not define them here. */
#include <stdint.h>  /* for uint64_t if needed, but idx_t is supplied */

/* External linkage required on the function itself. */
seq_t a(idx_t n)
{
    /* Fast path for the first four terms (offset 1). */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)6;
    if (n == 4) return (seq_t)16;

    /* Use a sliding window of the last four computed values. */
    seq_t a1 = (seq_t)1;  /* a(n-4) */
    seq_t a2 = (seq_t)2;  /* a(n-3) */
    seq_t a3 = (seq_t)6;  /* a(n-2) */
    seq_t a4 = (seq_t)16; /* a(n-1) */
    seq_t ai;             /* current a(n) */

    /* Compute from 5 up to n. */
    for (idx_t i = 5; i <= n; ++i) {
        ai = (seq_t)(2 * a4 + 8 * a1);

        /* Slide the window: a1 = a2, a2 = a3, a3 = a4, a4 = ai. */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = ai;
    }

    return ai;
}
