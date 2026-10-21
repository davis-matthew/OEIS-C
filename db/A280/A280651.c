/*
 * A280651  Numbers k such that k^3 has an odd number of digits in base 2 and
 *          the middle digit is 1.
 * https://oeis.org/A280651
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the n‑th positive integer k such that k^3 in base 2 has
 *            an odd number of digits and the middle digit equals 1
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 632, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n) {
    idx_t found = 0;   /* count of terms found so far   */
    seq_t k = 1;       /* candidate integer */

    while (found < n) {
        seq_t cube = k * k * k;            /* k³ */

        /* compute bit length of cube */
        unsigned bits = 0;
        seq_t temp = cube;
        while (temp) {
            bits++;
            temp >>= 1;
        }

        if ((bits & 1U) != 0) {             /* odd number of bits */
            unsigned mid = bits >> 1;       /* middlebit position */
            if (((cube >> mid) & 1U) == 1U) { /* middle bit is 1   */
                found++;
                if (found == n)
                    return k;                 /* desired term reached */
            }
        }
        ++k;                                 /* next candidate   */
    }
    return 0;                                 /* never reached    */
}
