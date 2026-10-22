/*
 * A328012  Numbers whose binary representations start and end with 1 and
 *          contain an even number of zeros between.
 * https://oeis.org/A328012
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th positive integer whose binary representation *
 *            starts and ends with 1 and contains an even number of zeros
 *            between. * The function performs a simple linear search over odd
 *            candidates, * testing the parity of the interior zeros. It
 *            returns the n‑th term * for all n ≥ 1
 * Range:     u8 n <= 65, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n = 0, guard */

    idx_t count = 0;
    for (idx_t cand = 1; ; cand += 2) {   /* iterate over odd numbers */
        idx_t tmp = cand >> 1;            /* drop the least‑significant 1 */
        bool parity_zero = false;        /* parity of zeros inside */

        while (tmp) {
            if ((tmp & 1) == 0)          /* current bit is 0 */
                parity_zero = !parity_zero;
            tmp >>= 1;
        }
        if (!parity_zero) {              /* even number of interior zeros */
            ++count;
            if (count == n)
                return (seq_t)cand;
        }
    }
}
