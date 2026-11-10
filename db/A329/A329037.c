/*
 * A329037  a(n) = Product_{d|n, d>1} A008578(1+A286561(A276086(n),d)), where
 *          A286561(x,d) gives the exponent of the highest power of d dividing
 *          x.
 * https://oeis.org/A329037
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{d|n, d>1} A008578(1+A286561(A276086(n),d)),
 *            where A276086(n) = Product_k prime(k+1)^(k-th primorial-base
 *            digit of n), A286561(x,d) = min over primes q^f || d of
 *            floor(v_q(x)/f), and A008578(1+e) = 1 if e = 0, prime(e)
 *            otherwise
 * Range:     u8 n <= 104, u16 n <= 769, u32 n <= 10009, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9656 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static unsigned long long A329037_prime_k(unsigned long long k)
{
    /* k-th prime, k >= 1, by trial division */
    unsigned long long c = 1, cnt = 0;
    while (cnt < k) {
        ++c;
        bool ok = true;
        for (unsigned long long q = 2; q * q <= c; ++q)
            if (c % q == 0) { ok = false; break; }
        if (ok) ++cnt;
    }
    return c;
}

/* exponent of prime q in A276086(n), given its primorial digits */
static unsigned long long A329037_exp_of(unsigned long long q,
                                         const unsigned long long *pr,
                                         const unsigned long long *dg, int K)
{
    for (int j = 0; j < K; ++j)
        if (pr[j] == q) return dg[j];
    return 0;
}

/* A286561(A276086(n), d) for d > 1 */
static unsigned long long A329037_val(idx_t d,
                                      const unsigned long long *pr,
                                      const unsigned long long *dg, int K)
{
    unsigned long long v = ~0ULL;
    idx_t m = d;
    for (idx_t q = 2; q <= m / q; ++q) {
        if (m % q != 0) continue;
        unsigned long long f = 0;
        while (m % q == 0) { m /= q; ++f; }
        unsigned long long r = A329037_exp_of((unsigned long long)q, pr, dg, K) / f;
        if (r < v) v = r;
    }
    if (m > 1) {
        unsigned long long r = A329037_exp_of((unsigned long long)m, pr, dg, K);
        if (r < v) v = r;
    }
    return v;
}

seq_t a(idx_t n)
{
    unsigned long long pr[64];
    unsigned long long dg[64];
    int K = 0;
    idx_t rem = n;
    while (rem > 0 && K < 64) {
        unsigned long long p = A329037_prime_k((unsigned long long)(K + 1));
        pr[K] = p;
        dg[K] = (unsigned long long)(rem % p);
        rem /= p;
        ++K;
    }

    seq_t result = 1;
    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i != 0) continue;
        idx_t ds[2];
        int nd = 0;
        ds[nd++] = i;
        if (i != n / i) ds[nd++] = n / i;
        for (int t = 0; t < nd; ++t) {
            idx_t d = ds[t];
            if (d == 1) continue;
            unsigned long long v = A329037_val(d, pr, dg, K);
            if (v > 0 && v != ~0ULL)
                result *= (seq_t)A329037_prime_k(v);
        }
    }
    return result;
}
