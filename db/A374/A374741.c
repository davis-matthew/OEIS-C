/*
 * A374741  Sum of leaders of weakly decreasing runs in the n-th composition
 *          in standard order.
 * https://oeis.org/A374741
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of leaders of weakly decreasing runs in the n-th
 *            composition in standard order
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Return the n-th term of OEIS A374741 */
seq_t a(idx_t n)
{
    /* composition of n is derived from the set bits of n */
    if (n == 0) return 0;     /* empty composition has sum 0 */

    /* gather positions (1‑based) of bits set in the binary representation of n,
       counting from the least significant bit */
    idx_t pos[64];            /* 64 bits are enough for idx_t (uint64_t)    */
    int pc = 0;               /* count of set bits                      */
    for (idx_t i = 0; i < 64 && n >> i; ++i) {
        if (n & ((idx_t)1 << i)) {
            pos[pc++] = i + 1;
        }
    }

    /* compute differences of successive positions; this gives the composition
       parts before reversing                                      */
    idx_t parts[64];
    for (int j = 0; j < pc; ++j) {
        if (j == 0)
            parts[j] = pos[j];             /* first element: pos - 0   */
        else
            parts[j] = pos[j] - pos[j - 1]; /* successive differences   */
    }

    /* now process the reversed composition to sum leaders of weakly
       decreasing runs.  We iterate from the end of 'parts' to its
       beginning because reversing the array is equivalent to this.
       The first element of the reversed sequence is parts[pc-1].   */
    seq_t sum = (seq_t)parts[pc - 1];            /* first leader      */
    for (int i = pc - 2; i >= 0; --i) {
        idx_t curr = parts[i];
        idx_t prev = parts[i + 1];              /* previous element in reversed order */
        if (curr > prev) {                      /* start of a new run        */
            sum += (seq_t)curr;
        }
    }
    return sum;
}
