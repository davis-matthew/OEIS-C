/*
 * A373789  Indices of record high points in A372072.
 * https://oeis.org/A373789
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 58, u16 n <= 6546, u32 n <= 9928, u64 n <= 9928,
 *            u128 n <= 9928, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) = the n-th smallest integer that is either 1, 4, 6, 10 or a prime. */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 0)   /* Undefined for offset 1, but keep safe */
        return 0;

    idx_t   count = 0;          /* Number of qualifying values found */
    seq_t   num    = 0;          /* Current integer to test */

    for (num = 1; ; ++num) {
        bool qual = (num == 1 || num == 4 || num == 6 || num == 10
                     || is_prime((idx_t)num));

        if (qual) {
            ++count;
            if (count == n)
                return num;
        }
    }
}
