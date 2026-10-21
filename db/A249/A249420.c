/*
 * A249420  Least number m>1 such that m*prime(n)-2 is prime.
 * https://oeis.org/A249420
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the least m>1 such that m*prime(n)-2 is prime
 * Range:     u8 n <= 9, u16 n <= 730, u32 n <= 798, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* OEIS: offset 1, a(1) is the first term */
    if (n < 1) return 0;                 /* undefined for n<1, return 0 */
    seq_t prime = (seq_t)get_prime(n);   /* nth prime */
    seq_t m = 2;

    while (1) {
        /* compute m*prime-2 in a wider type to avoid overflow on
         * intermediate calculations, then store back into seq_t */
        unsigned long long tmp = (unsigned long long)m * (unsigned long long)prime - 2ULL;
        seq_t val = (seq_t)tmp;
        if (is_prime_big(val))
            return m;
        ++m;
    }
}
