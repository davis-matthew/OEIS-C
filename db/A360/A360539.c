/*
 * A360539  a(n) is the cubefree part of n: the largest unitary divisor of n
 *          that is a cubefree number (A004709).
 * https://oeis.org/A360539
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = product_{p^e || n} p^min(e,2) (cubefree part of n)
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    uint64_t x = (uint64_t)n;
    seq_t res = (seq_t)1;

    /* handle factor 2 */
    uint64_t exp = 0;
    while ((x & 1ULL) == 0) {
        x >>= 1;
        exp++;
    }
    if (exp > 0 && exp <= 2) {
        seq_t pp = (seq_t)2;
        if (exp == 2)
            pp *= (seq_t)2;
        res *= pp;
    }

    /* handle odd factors */
    for (uint64_t p = 3; p * p <= x; p += 2) {
        if (x % p == 0) {
            exp = 0;
            while (x % p == 0) {
                x /= p;
                exp++;
            }
            if (exp > 0 && exp <= 2) {
                seq_t pp = (seq_t)p;
                if (exp == 2)
                    pp *= (seq_t)p;
                res *= pp;
            }
        }
    }

    /* remaining prime factor, if any */
    if (x > 1) {
        /* exponent is 1 */
        res *= (seq_t)x;
    }

    return res;
}
