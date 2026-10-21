/*
 * A279622  Numbers with a prime factor greater than 5.
 * https://oeis.org/A279622
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   numbers that have a prime factor greater than 5 (i.e., all
 *            positive integers except those of the form 2^a 3^b 5^c)
 * Range:     u8 n <= 204, u16 n <= 9825, u32 n <= 9825, u64 n <= 9825,
 *            u128 n <= 9825, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t count = 0;               /* how many numbers with a prime factor >5 found */
    for (seq_t cand = 1;; ++cand) {
        seq_t m = cand;
        /* remove all factors 2, 3 and 5 */
        while (m % 2 == 0)  m /= 2;
        while (m % 3 == 0)  m /= 3;
        while (m % 5 == 0)  m /= 5;
        /* if anything remains, cand has a prime factor >5 */
        if (m > 1) {
            ++count;
            if (count == n)
                return cand;
        }
    }
}
