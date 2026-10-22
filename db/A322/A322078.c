/*
 * A322078  a(n) = n^2 * Sum_{p|n, p prime} 1/p^2.
 * https://oeis.org/A322078
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^2 * Sum_{p|n, p prime} 1/p^2 (OEIS A322078)
 * Range:     u8 n <= 15, u16 n <= 419, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Work with the actual integer value first. */
    uint64_t nn = (uint64_t)n;          /* n is a uint64_t due to idx_t definition */
    seq_t n_sq = (seq_t)nn * (seq_t)nn; /* n^2 */

    uint64_t temp = nn; /* number to factor */
    seq_t  result = 0;

    /* Handle factor 2 separately to allow skipping even numbers later. */
    if ((temp & 1u) == 0) {
        result += n_sq / (seq_t)4;          /* divide by 2^2 = 4 */
        while ((temp & 1u) == 0)
            temp >>= 1;                    /* temp /= 2 */
    }

    /* Trial division by odd numbers up to sqrt(temp). */
    for (uint64_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            seq_t pp = (seq_t)p * (seq_t)p;    /* p^2 in seq_t */
            result += n_sq / pp;
            while (temp % p == 0)
                temp /= p;                     /* remove all powers of p */
        }
    }

    /* If a prime > sqrt(original n) remains, it is a single factor. */
    if (temp > 1) {
        seq_t pp = (seq_t)temp * (seq_t)temp;
        result += n_sq / pp;
    }

    return result;
}
