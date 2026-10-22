/*
 * A354579  Number of distinct lengths of runs in the n-th composition in
 *          standard order.
 * https://oeis.org/A354579
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A354579
    a(n) = number of distinct run-lengths of the
           n‑th composition in standard (graded reverse‑lexicographic) order.
*/

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* empty composition → 0 */

    /* build composition from the bits of n */
    /* each set bit gives a part; parts are stored in ascending bit order,
       then reversed to obtain the composition.                 */
    uint64_t parts[64];
    size_t   len   = 0;
    uint64_t prev_pos = 0;

    for (size_t i = 0; i < 64; ++i) {
        if ((n >> i) & (idx_t)1) {
            parts[len++] = (i + 1) - prev_pos;
            prev_pos = i + 1;
        }
    }

    if (len == 0) return (seq_t)0; /* never occurs for n>0, safety */

    /* reverse parts array to get the composition */
    for (size_t i = 0; i < len / 2; ++i) {
        uint64_t tmp = parts[i];
        parts[i] = parts[len - 1 - i];
        parts[len - 1 - i] = tmp;
    }

    /* find distinct run lengths */
    uint64_t runmask = 0;
    size_t   runlen  = 1;

    for (size_t i = 0; i < len; ++i) {
        if (i + 1 == len || parts[i] != parts[i + 1]) {
            runmask |= ((uint64_t)1 << (runlen - 1));
            runlen = 1;
        } else {
            ++runlen;
        }
    }

    /* count set bits in runmask → number of distinct run lengths */
    size_t count = 0;
    while (runmask) {
        count += runmask & 1;
        runmask >>= 1;
    }

    return (seq_t)count;
}
