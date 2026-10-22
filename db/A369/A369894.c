/*
 * A369894  a(n) = n * Sum_{p|n, p prime} tau(n/p) / p.
 * https://oeis.org/A369894
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 2519, u32 n <= 9338, u64 n <= 9338,
 *            u128 n <= 9338, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = Sum_{p|n} (n/p) * tau(n/p) 
 * where the sum is over all distinct prime divisors p of n and 
 * tau(k) is the number of positive divisors of k.
 */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 1)          /* a(1) = 0 */
        return (seq_t)0;

    uint64_t nn = (uint64_t)n;          /* for long‑loop calculations */
    seq_t tau_n = 1;                    /* tau(n) */

    /* Temporary storage for prime factors and exponents (max < 64 for 64‑bit n) */
    uint64_t primes[64];
    int       exps   [64];
    int       cnt    = 0;

    uint64_t m = nn;
    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            int e = 0;
            while (m % p == 0) { m /= p; e++; }
            primes[cnt] = p;
            exps  [cnt] = e;
            tau_n *= (seq_t)(e + 1);
            ++cnt;
        }
    }
    if (m > 1) {                       /* remaining prime factor */
        primes[cnt] = m;
        exps  [cnt] = 1;
        tau_n *= 2;
        ++cnt;
    }

    seq_t result = 0;
    for (int i = 0; i < cnt; ++i) {
        seq_t p = (seq_t)primes[i];
        int   e = exps[i];

        /* term = (n/p) * tau(n/p)  with  tau(n/p) = tau_n * e / (e+1) */
        seq_t term = (seq_t)(nn / (uint64_t)p);   /* n/p, integer division */
        term *= tau_n;
        term *= (seq_t)e;
        term /= (seq_t)(e + 1);

        result += term;
    }
    return result;
}
