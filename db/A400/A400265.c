/*
 * A400265  The third least significant digit of n' [the arithmetic derivative
 *          of n, A003415] in the primorial base, A049345.
 * https://oeis.org/A400265
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the third least significant digit (base primorial) of the
 *            arithmetic derivative n', i.e. the coefficient of 6 in the
 *            primorial base representation of n'
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>            /* for uint64_t */

seq_t a(idx_t n)
{
    /* For n = 0 or 1, arithmetic derivative is 0 → answer is 0. */
    if (n <= 1)
        return (seq_t)0;

    /* Compute n' (the arithmetic derivative) modulo 30. */
    uint64_t nn = (uint64_t)n;          /* integer value of n */
    uint64_t tmp = nn;
    uint64_t dmod = 0;                  /* n' modulo 30 */

    /* Factor n by trial division up to sqrt(n). */
    for (uint64_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            int e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            /* term = e * n / p  (this is an integer) */
            uint64_t term = (uint64_t)e * (nn / p);
            dmod = (dmod + term) % 30;
        }
    }
    if (tmp > 1) {   /* remaining prime factor */
        uint64_t p = tmp;
        uint64_t term = nn / p;   /* e = 1 */
        dmod = (dmod + term) % 30;
    }

    /* Decode the third least significant primorial digit from dmod. */
    uint64_t r   = dmod;          /* 0 <= r < 30 */
    uint64_t d0  = r & 1;         /* r mod 2 */
    uint64_t r1  = (r - d0) >> 1; /* (r - d0)/2  -> 0..14 */
    uint64_t d1  = r1 % 3;        /* r1 mod 3   -> 0..2 */
    uint64_t r2  = (r1 - d1) / 3; /* (r1 - d1)/3 -> 0..4 */
    uint64_t d2  = r2;            /* third digit (0..4) */

    return (seq_t)d2;
}
