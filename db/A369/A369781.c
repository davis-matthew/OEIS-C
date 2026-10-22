/*
 * A369781  a(n) = number of nonempty subsets S of {1,2,...,n} such that
 *          (number of primes in S) <= (number of nonprimes in S).
 * https://oeis.org/A369781
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^n - 1 - sum_{a>b} C(p,a) * C(q,b) where p = number of
 *            primes ≤ n and q = n - p
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/pow2.h"

/* helper: count the primes up to n (inclusive) using a simple sieve */
static int count_primes_upto(idx_t n)
{
    if (n < 2) return 0;
    size_t m = (size_t)n + 1;
    uint8_t *is_prime = (uint8_t*)malloc(m);
    if (!is_prime) abort();
    for (size_t i = 0; i < m; ++i) is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;
    for (size_t i = 2; i * i <= (size_t)n; ++i)
        if (is_prime[i])
            for (size_t j = i * i; j <= (size_t)n; j += i)
                is_prime[j] = 0;
    int cnt = 0;
    for (size_t i = 2; i <= (size_t)n; ++i) if (is_prime[i]) ++cnt;
    free(is_prime);
    return cnt;
}

/* main function as required */
seq_t a(idx_t n)
{
    /* count primes and non‑primes */
    int p = count_primes_upto(n);
    int q = (int)n - p;            /* q >= 0 */

    /* Cq[b] = binom(q,b), pref[b] = sum_{i=0..b} Cq[i] */
    seq_t *Cq   = (seq_t*)calloc((size_t)q + 1, sizeof(seq_t));
    seq_t *pref = (seq_t*)calloc((size_t)q + 1, sizeof(seq_t));
    if (!Cq || !pref) abort();

    Cq[0] = 1;
    pref[0] = 1;
    for (int b = 1; b <= q; ++b) {
        /* Cq[b] = Cq[b-1] * (q - b + 1) / b */
        seq_t tmp = Cq[b - 1] * (seq_t)(q - b + 1);
        Cq[b] = tmp / (seq_t)b;
        pref[b] = pref[b - 1] + Cq[b];
    }

    /* Sum over a > b: F = Σ_{a=1..p} C(p,a) * pref[a-1] */
    seq_t F = 0;
    seq_t Cp = 1;                 /* C(p,0) */
    for (int a = 1; a <= p; ++a) {
        /* Cp = Cp * (p - a + 1) / a -> C(p,a) */
        seq_t tmp = Cp * (seq_t)(p - a + 1);
        Cp = tmp / (seq_t)a;
        F += Cp * pref[a - 1];
    }

    free(Cq);
    free(pref);

    /* a(n) = 2^n - 1 - F */
    seq_t pow2n = pow2(n);
    seq_t result = pow2n - 1 - F;
    return result;
}
