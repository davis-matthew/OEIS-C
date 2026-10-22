/*
 * A326142  Sum of all other divisors of n except its largest squarefree
 *          divisor: a(n) = sigma(n) - A007947(n).
 * https://oeis.org/A326142
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sigma(n) - rad(n)
 * Range:     u8 n <= 107, u16 n <= 9394, u32 n <= 9394, u64 n <= 9394,
 *            u128 n <= 9394, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* seq_t and idx_t are defined by the build system   */

#include <stdint.h>

/* Return the required value for the OEIS sequence A326142.  */
seq_t a(idx_t n)
{
    if (n == 0)               /* safety, although OEIS offset starts at 1 */
        return (seq_t)0;

    idx_t tmp = n;            /* remaining part to factorise      */
    seq_t sigma = (seq_t)1;   /* product of the σ‑contributions   */
    seq_t rad   = (seq_t)1;   /* product of the distinct primes   */

    idx_t i = 2;
    while ((unsigned __int128)i * i <= (unsigned __int128)tmp) {
        if (tmp % i == 0) {
            /*  i^e  is a prime power dividing n   */
            seq_t term  = (seq_t)1;   /* sum of powers of i: 1 + i + ... + i^e  */
            seq_t ppow  = (seq_t)1;   /* i^k for the current k                */

            do {
                tmp /= i;            /* remove one factor i                */
                ppow *= (seq_t)i;    /* update i^k                          */
                term += ppow;        /* add to the sum                      */
            } while (tmp % i == 0);

            sigma *= term;          /* multiply into σ(n)                  */
            rad   *= (seq_t)i;      /* multiply into rad(n)                */
        }
        ++i;
    }

    /* Any remaining prime factor > sqrt(original n) */
    if (tmp > 1) {
        /*   σ contribution: 1 + p   */
        sigma *= (seq_t)(1 + tmp);
        rad   *= (seq_t)tmp;
    }

    return sigma - rad;          /* a(n) = σ(n) - rad(n)                 */
}
