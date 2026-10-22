/*
 * A336643  Squarefree kernel of n divided by the squarefree part of n: a(n) =
 *          rad(n) / core(n).
 * https://oeis.org/A336643
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = rad(n) / core(n)
 * Range:     u8 n <= 9515, u16 n <= 9515, u32 n <= 9515, u64 n <= 9515,
 *            u128 n <= 9515, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) { /* Undefined offset; return 1 for safety */
        return (seq_t)1;
    }
    uint64_t nn = (uint64_t)n;
    seq_t product = (seq_t)1;

    /* Handle factor 2 */
    unsigned int exp = 0;
    while (nn % 2 == 0) {
        exp++;
        nn /= 2;
    }
    if (exp % 2 == 0 && exp > 0) {
        product *= (seq_t)2;
    }

    /* Handle odd prime factors */
    for (uint64_t p = 3; p <= nn / p; p += 2) {
        if (nn % p != 0) {
            continue;
        }
        unsigned int e = 0;
        while (nn % p == 0) {
            e++;
            nn /= p;
        }
        if (e % 2 == 0 && e > 0) {
            product *= (seq_t)p;
        }
    }

    /* Any remaining prime factor > sqrt(nn) has exponent 1 (odd) – ignore it */

    return product;
}
