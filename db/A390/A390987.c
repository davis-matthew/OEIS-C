/*
 * A390987  Irregular triangle read by rows where row k lists the first sums
 *          of binary indices of n.
 * https://oeis.org/A390987
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, tabf, changed
 * Formula:   not stated
 * Range:     u8 n <= 9334, u16 n <= 9334, u32 n <= 9334, u64 n <= 9334,
 *            u128 n <= 9334, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) is the n-th element of the concatenated list of the first
   sums of binary indices of integers starting from 3. Each number m contributes
   a sequence of sums of consecutive binary indices of m (positions counted
   from 1 in the reversed binary expansion). The triangle is read by rows,
   so the global index n is obtained by walking through m in order and
   emitting each sum until the n-th element is reached. */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 3) return 0;          /* undefined below offset, safe default */

    /* The first element (n = 3) corresponds to the 1st sum produced. */
    idx_t target = n - 2;         /* 1‑based position in the concatenation */
    idx_t cnt = 0;                /* number of sums emitted so far */
    idx_t m = 3;                  /* start scanning numbers from 3 upwards */

    while (1) {
        idx_t tmp = m;
        idx_t bitPos = 1;         /* position of the current bit, starting at 1 */
        idx_t prevPos = 0;        /* last seen position of a 1‑bit */
        int hasPrev = 0;          /* true when prevPos holds a value */

        /* Scan bits of m from least significant to most significant. */
        while (tmp) {
            if (tmp & 1) {        /* this bit is set (1) */
                if (hasPrev) {
                    seq_t val = (seq_t)prevPos + (seq_t)bitPos;
                    cnt++;
                    if (cnt == target) return val;
                }
                prevPos = bitPos;
                hasPrev = 1;
            }
            tmp >>= 1;
            bitPos++;
        }
        m++;
    }
}
