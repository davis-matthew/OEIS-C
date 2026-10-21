/*
 * A298185  Number of nX6 0..1 arrays with every element equal to 3, 5, 7 or 8
 *          king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A298185
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 25, u32 n <= 48, u64 n <= 94, u128 n <= 187,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A298185
   Definition: Number of n×6 0..1 arrays with every element equal to 3, 5, 7 or 8 king‑move adjacent elements,
   with upper left element zero.
   Recurrence (valid for n > 8):
     a(n) = 3*a(n−1) − 2*a(n−2) − a(n−3) + 2*a(n−4) − 2*a(n−5) + a(n−7)
   Initial values:
     a(1)=0,  a(2)=5,  a(3)=5,  a(4)=6,  a(5)=7,
     a(6)=10, a(7)=14, a(8)=20
*/

#include <stddef.h>
#include <stdlib.h>

/* Compute a(n) for n >= 1 */
seq_t a(idx_t n)
{
    /* Initial terms: index 1..8 */
    static const seq_t init[9] = {0, 0, 5, 5, 6, 7, 10, 14, 20};

    /* Static cache large enough for n up to about 10000 */
    static seq_t cache[10010] = {0};
    static idx_t last_computed = 0;   /* last n for which cache is valid */

    /* Return immediately if we only need a small initial value */
    if (n <= 8)
        return init[n];

    /* Initialise cache with the first 8 values on the first call */
    if (last_computed == 0) {
        for (idx_t i = 1; i <= 8; ++i)
            cache[i] = init[i];
        last_computed = 8;
    }

    /* Compute new values up to n */
    if (n > last_computed) {
        for (idx_t i = last_computed + 1; i <= n; ++i) {
            /* Use unsigned arithmetic; the sequence guarantees the
               intermediate sums stay non‑negative for the given n. */
            seq_t term = 3 * cache[i - 1];
            term -= 2 * cache[i - 2];
            term -= cache[i - 3];
            term += 2 * cache[i - 4];
            term -= 2 * cache[i - 5];
            term += cache[i - 7];
            cache[i] = term;
        }
        last_computed = n;
    }

    return cache[n];
}
