/*
 * A370853  Numbers m such that c(0) < c(1) < c(2), where c(k) = number of k's
 *          in the ternary representation of m.
 * https://oeis.org/A370853
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n‑th positive integer m that satisfies c0(m) < c1(m)
 *            < c2(m), where cj(m) is the number of digit j in the ternary
 *            representation of m
 * Range:     u8 n <= 22, u16 n <= 7326, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n) {
    if (n < 1) return 0;          /* undefined for n < 1, return 0 */

    idx_t found = 0;              /* number of terms found so far */
    seq_t m = 1;                  /* current integer to test */

    while (found < n) {
        unsigned int c0 = 0, c1 = 0, c2 = 0;

        if (m == 0) {
            /* representation of 0 is the single digit 0 */
            c0 = 1;
        } else {
            seq_t tmp = m;
            while (tmp > 0) {
                unsigned int d = (unsigned int)(tmp % 3);
                /* Count digit d */
                if (d == 0)     c0++;
                else if (d == 1) c1++;
                else             c2++;    /* d == 2 */
                tmp /= 3;
            }
        }

        if (c0 < c1 && c1 < c2) {
            ++found;
            if (found == n)
                return m;
        }

        ++m; /* move to next integer */
    }

    /* control never reaches here */
    return 0;
}
