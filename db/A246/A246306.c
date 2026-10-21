/*
 * A246306  Numbers k such that cos(k) > cos(k+1) > cos(k+2) > cos(k+3) <
 *          cos(k+4).
 * https://oeis.org/A246306
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Numbers k such that cos(k)>cos(k+1)>cos(k+2)>cos(k+3)<cos(k+4)
 * Range:     u8 n <= 41, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdbool.h>

/* Return the n-th element (1‑based) of sequence A246306. */
seq_t a(idx_t n) {
    /* Scan successive integers and count matches of the cosine pattern. */
    idx_t k = 0;
    idx_t count = 0;       /* number of pattern hits found so far */

    while (1) {
        double c0 = cos((double)k);
        double c1 = cos((double)(k + 1));
        double c2 = cos((double)(k + 2));
        double c3 = cos((double)(k + 3));
        double c4 = cos((double)(k + 4));

        if (c0 > c1 && c1 > c2 && c2 > c3 && c3 < c4) {
            ++count;
            if (count == n) {
                return (seq_t)k;   /* found the n‑th term */
            }
        }
        ++k;   /* move to the next integer */
    }
}
