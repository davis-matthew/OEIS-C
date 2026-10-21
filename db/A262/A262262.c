/*
 * A262262  Number of prime divisors p | n such that p^2 < n and p^2 does not
 *          divide n.
 * https://oeis.org/A262262
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of prime divisors p of n such that p^2 < n and n %
 *            (p^2) != 0
 * Range:     u8 n <= 9625, u16 n <= 9625, u32 n <= 9625, u64 n <= 9625,
 *            u128 n <= 9625, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;

    uint64_t n_orig = n;
    uint64_t tmp = n_orig;
    seq_t count = 0;

    /* handle factor 2 */
    if (tmp % 2 == 0) {
        while (tmp % 2 == 0)
            tmp /= 2;
        uint64_t sq = 4ULL;                     /* 2^2 */
        if (sq < n_orig && n_orig % sq != 0)
            count++;
    }

    /* handle odd prime factors */
    for (uint64_t p = 3; p <= tmp / p; p += 2) {
        if (tmp % p == 0) {
            while (tmp % p == 0)
                tmp /= p;
            uint64_t sq = p * p;
            if (sq < n_orig && n_orig % sq != 0)
                count++;
        }
    }

    /* remaining prime factor > sqrt(n_orig) */
    if (tmp > 1) {
        uint64_t sq = tmp * tmp;
        if (sq < n_orig && n_orig % sq != 0)
            count++;
    }

    return count;
}
