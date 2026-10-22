/*
 * A356006  The number of prime divisors of n that are not greater than 5,
 *          counted with multiplicity.
 * https://oeis.org/A356006
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of prime factors of n that are <=5, counted with
 *            multiplicity
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* The sequence is totally additive with a(p)=1 for p=2,3,5 and a(p)=0 otherwise.
       Therefore we simply count the multiplicities of 2, 3 and 5 in n. */
    if (n <= 1)
        return 0;

    seq_t count = 0;

    /* Count factors of 2 */
    while ((n & 1ULL) == 0)
    {
        count++;
        n >>= 1;
    }

    /* Count factors of 3 */
    while (n % 3 == 0)
    {
        count++;
        n /= 3;
    }

    /* Count factors of 5 */
    while (n % 5 == 0)
    {
        count++;
        n /= 5;
    }

    return count;
}
