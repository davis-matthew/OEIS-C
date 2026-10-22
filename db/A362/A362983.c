/*
 * A362983  Number of prime factors of n (with multiplicity) that are greater
 *          than the least.
 * https://oeis.org/A362983
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of prime factors of n (with multiplicity) that are
 *            greater than the least prime factor (a(1)=0)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* a(1) is defined as 0 */
    if (n <= 1)
        return 0;

    /* Find the least prime factor */
    uint64_t tmp = (uint64_t)n;
    uint64_t minpf = tmp;
    for (uint64_t i = 2; i <= tmp / i; ++i) {
        if (tmp % i == 0) {
            minpf = i;
            break;
        }
    }

    /* Count prime factors > minpf */
    seq_t count = 0;
    tmp = (uint64_t)n;
    while (tmp > 1) {
        /* Find next smallest prime factor */
        uint64_t f = tmp;
        for (uint64_t i = 2; i <= tmp / i; ++i) {
            if (tmp % i == 0) {
                f = i;
                break;
            }
        }

        if (f == minpf) {
            /* Skip all occurrences of the least prime factor */
            while (tmp % f == 0)
                tmp /= f;
        } else {
            /* Count all occurrences of this factor */
            while (tmp % f == 0) {
                count++;
                tmp /= f;
            }
        }
    }

    return count;
}
