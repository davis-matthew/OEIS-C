/*
 * A338651  Number of divisors of n which are greater than 7.
 * https://oeis.org/A338651
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of divisors of n greater than 7
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;                /* offset starts at 1 */

    /* total number of divisors of n */
    idx_t m = n;
    seq_t divCnt = 1;

    /* factorization by trial division */
    for (idx_t i = 2; i <= m / i; ++i) {
        if (m % i == 0) {
            idx_t exp = 0;
            while (m % i == 0) {
                m /= i;
                ++exp;
            }
            divCnt *= (seq_t)(exp + 1);
        }
    }
    if (m > 1) divCnt *= (seq_t)2;  /* remaining prime factor */

    /* count small divisors (1 through 7) that divide n */
    static const idx_t small_divs[7] = {1, 2, 3, 4, 5, 6, 7};
    idx_t smallCnt = 0;
    for (size_t k = 0; k < sizeof(small_divs)/sizeof(small_divs[0]); ++k) {
        if (n % small_divs[k] == 0)
            ++smallCnt;
    }

    return divCnt - (seq_t)smallCnt;
}
