/*
 * A374630  Sum of leaders of weakly increasing runs in the n-th composition
 *          in standard order.
 * https://oeis.org/A374630
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of leaders of weakly increasing runs in the n-th *
 *            composition in standard order. * The composition is obtained
 *            from the binary representation of n: * - Let p1 < p2 < … < pk be
 *            the positions (1‑based from the least * significant bit) of the
 *            set bits of n. * - Compute differences d0 = p...
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* External linkage as required */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                   /* empty composition */

    unsigned diff[64];                       /* differences between positions */
    unsigned m = 0;                          /* number of differences */
    unsigned last = 0;                       /* last seen position (0 at start) */

    /* gather set bit positions (1‑based) and compute differences */
    for (unsigned i = 0; i < 64; ++i) {
        if ((n >> i) & 1ULL) {
            unsigned pos = i + 1;
            diff[m++] = pos - last;          /* gap from previous position */
            last = pos;
        }
    }

    seq_t sum = 0;                          /* result */
    seq_t prev = 0;                          /* previous part in composition */
    bool started = false;

    /* traverse the reversed differences (the composition) */
    for (int i = (int)m - 1; i >= 0; --i) {
        seq_t val = (seq_t)diff[i];
        if (!started || val < prev)        /* start of a new run */
            sum += val;
        prev = val;
        started = true;
    }

    return sum;
}
