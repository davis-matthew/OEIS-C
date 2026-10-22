/*
 * A392598  The largest 11-smooth divisor of n.
 * https://oeis.org/A392598
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = the largest 11‑smooth divisor of n (product of all prime
 *            factors of n that are ≤ 11)
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined for n=0, return 0 as a safe guard */

    static const idx_t primes[5] = {2, 3, 5, 7, 11};
    const idx_t np = 5;

    seq_t res = 1;
    idx_t t = n;
    idx_t i;
    for (i = 0; i < np; ++i) {
        idx_t p = primes[i];
        while (t % p == 0) {
            res *= (seq_t)p;
            t /= p;
        }
    }
    return res;
}
