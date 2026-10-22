/*
 * A335834  Sort the run lengths in binary expansion of n in ascending order
 *          (with multiplicities).
 * https://oeis.org/A335834
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look, easy
 * Formula:   not stated
 * Range:     u8 n <= 255, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   _sort_ascending
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A335834 – sort the run lengths in the binary expansion of n
 *  and re‑create a binary number starting with a run of 1’s.
 *  For n > 0:
 *      1.  Write n in binary.
 *      2.  List the lengths of consecutive runs of identical bits.
 *          Sort these lengths in ascending order.
 *      3.  Rebuild a binary number whose run lengths are the sorted
 *          list, starting with a run of 1’s, then 0’s, alternating.
 *      4.  Return that new binary number.
 *  For n = 0 the sequence is defined to be 0 (a(0) = 0).
 *
 *  The operation is idempotent (a(a(n)) = a(n)).
 */
#include <stdint.h>
#include <stdlib.h>
#include "../utils/_sort_ascending.h"

/* Main function implementing A335834.  */
seq_t a(idx_t n)
{
    /* Special case for zero.  */
    if (n == 0)
        return (seq_t)0;

    /* Determine the most significant bit position.  */
    const int msb = 63 - __builtin_clzll((unsigned long long)n);

    /* Compute run lengths.  */
    int runs[65];            /* Max runs ≤ bits of n (≤64). */
    int rc = 0;              /* Count of runs.  */
    int curr_bit = (int)((n >> msb) & 1ULL);
    int len = 1;

    for (int i = msb - 1; i >= 0; --i) {
        int bit = (int)((n >> i) & 1ULL);
        if (bit == curr_bit) {
            ++len;
        } else {
            runs[rc++] = len;
            curr_bit   = bit;
            len        = 1;
        }
    }
    runs[rc++] = len;

    /* Sort run lengths.  */
    _sort_ascending(runs, rc);

    /* Reconstruct the new binary number.  */
    seq_t result = 0;
    for (int i = 0; i < rc; ++i) {
        int bit = (i & 1) ? 0 : 1;   /* 1,0,1,0,...  */
        for (int j = 0; j < runs[i]; ++j) {
            result = (result << 1) | (seq_t)bit;
        }
    }
    return result;
}
