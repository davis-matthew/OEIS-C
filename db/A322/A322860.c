/*
 * A322860  Characteristic function of practical numbers, A005153: If n is in
 *          A005153, a(n) = 1, otherwise a(n) = 0.
 * https://oeis.org/A322860
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [A030057(n) > n] – characteristic function of practical
 *            numbers (A005153). A(n)=1 iff *n* is practical
 * Range:     u8 n <= 9935, u16 n <= 9935, u32 n <= 9935, u64 n <= 9935,
 *            u128 n <= 9935, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* 1 is practical by definition */
    if (n == 1)
        return 1;

    /* Practical numbers are all even; odd n cannot be practical. */
    if ((n & 1) != 0)
        return 0;

    /* Factorise n by trial division. */
    idx_t remainder = n;
    idx_t primes[32];
    unsigned int exps[32];
    size_t pcnt = 0;

    /* Handle factor 2 separately. */
    if ((remainder & 1) == 0) {
        unsigned int e = 0;
        while ((remainder & 1) == 0) {
            remainder >>= 1;
            ++e;
        }
        primes[pcnt] = 2;
        exps[pcnt] = e;
        ++pcnt;
    }

    /* Remaining odd factors. */
    for (idx_t d = 3; d * d <= remainder; d += 2) {
        if (remainder % d == 0) {
            unsigned int e = 0;
            while (remainder % d == 0) {
                remainder /= d;
                ++e;
            }
            primes[pcnt] = d;
            exps[pcnt] = e;
            ++pcnt;
        }
    }
    if (remainder > 1) {           /* last prime factor */
        primes[pcnt] = remainder;
        exps[pcnt] = 1;
        ++pcnt;
    }

    /* If the first prime is not 2, n is not practical. */
    if (pcnt == 0 || primes[0] != 2)
        return 0;

    /* Compute σ of the processed part and check the practical criterion. */
    seq_t s = 1;                    /* σ(1) = 1 */

    for (size_t i = 0; i < pcnt; ++i) {
        seq_t p = (seq_t)primes[i];
        unsigned int e = exps[i];

        /* Prerequisite: p <= 1 + σ(previous product) → σ >= p - 1 */
        if (s < p - 1)
            return 0;

        /* σ(p^e) = 1 + p + p^2 + ... + p^e */
        seq_t sigma_pe = 1;
        seq_t p_power = p;
        for (unsigned int k = 1; k <= e; ++k) {
            sigma_pe += p_power;
            if (k < e)
                p_power *= p;
        }

        /* Update σ(product) = previous σ * σ(p^e). */
        s *= sigma_pe;
    }

    return 1;          /* n is practical */
}
