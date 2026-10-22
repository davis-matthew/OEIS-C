/*
 * A352052  Sum of the 6th powers of the divisor complements of the odd proper
 *          divisors of n.
 * https://oeis.org/A352052
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^6 * Sum_{d|n, d<n, d odd} 1 / d^6
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 40, u64 n <= 1625,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* out of offset – harmless */

    /* odd part of n */
    idx_t odd = n;
    while ((odd & 1) == 0)
        odd >>= 1;

    /* factor the odd part by trial division */
    idx_t primes[32];
    idx_t exps[32];
    idx_t nf = 0;
    idx_t temp = odd;

    for (idx_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            idx_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            primes[nf] = p;
            exps[nf] = e;
            ++nf;
        }
    }
    if (temp > 1) {                    /* remaining prime factor */
        primes[nf] = temp;
        exps[nf] = 1;
        ++nf;
    }

    /* generate all odd divisors of the odd part */
    seq_t divisors[256];
    idx_t nd = 1;
    divisors[0] = (seq_t)1;

    for (idx_t fi = 0; fi < nf; ++fi) {
        idx_t p = primes[fi];
        idx_t e = exps[fi];
        idx_t cur_nd = nd;
        seq_t mul = (seq_t)1;
        for (idx_t k = 1; k <= e; ++k) {
            mul *= (seq_t)p;                     /* p^k */
            for (idx_t j = 0; j < cur_nd; ++j) {
                divisors[nd++] = divisors[j] * mul;
            }
        }
    }

    /* sum over odd proper divisors */
    seq_t sum = 0;
    seq_t nseq = (seq_t)n;

    for (idx_t i = 0; i < nd; ++i) {
        seq_t d = divisors[i];
        if (d == nseq) continue;             /* skip n itself if odd */
        seq_t t = (seq_t)(n / (idx_t)d);
        seq_t pow = t;
        for (idx_t k = 0; k < 5; ++k) pow *= t; /* t^6 */
        sum += pow;
    }

    return sum;
}
