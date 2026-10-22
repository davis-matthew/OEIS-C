/*
 * A306369  a(n) = A000010(n) + A069359(n).
 * https://oeis.org/A306369
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000010(n) + A069359(n) * where A069359(n) = sum_{p|n}
 *            n/p (sum over distinct prime divisors of n)
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Compute a(n) = φ(n) + sum_{p|n} n/p */
seq_t a(idx_t n)
{
    /* n is 1-indexed; n >= 1 */
    uint64_t n64 = (uint64_t)n;
    uint64_t phi = n64;          /* initialize φ(n) */
    uint64_t tmp  = n64;         /* factorization working copy */

    seq_t sum_over_p = 0;        /* accumulate A069359(n) */

    /* handle factor 2 separately for efficiency */
    if ((tmp & 1ULL) == 0) {            /* if divisible by 2 */
        while ((tmp & 1ULL) == 0)
            tmp >>= 1;                  /* remove all 2's */
        phi = phi / 2 * 1;              /* φ: divide by 2 and multiply by 1 (=2-1) */
        sum_over_p += (seq_t)(n64 / 2);
    }

    /* iterate over odd potential factors up to sqrt(tmp) */
    uint64_t p;
    for (p = 3; p * p <= tmp; p += 2) {
        if (tmp % p == 0) {
            while (tmp % p == 0)      /* strip all powers of p */
                tmp /= p;
            phi = phi / p * (p - 1);   /* update φ */
            sum_over_p += (seq_t)(n64 / p);  /* add n/p */
        }
    }

    /* if a prime factor remains > sqrt(original n) */
    if (tmp > 1) {
        phi = phi / tmp * (tmp - 1);
        sum_over_p += (seq_t)(n64 / tmp);
    }

    /* a(n) = φ(n) + A069359(n) */
    sum_over_p += (seq_t)phi;
    return sum_over_p;
}
