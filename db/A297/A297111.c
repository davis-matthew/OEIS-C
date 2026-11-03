/*
 * A297111  Möbius transform of A005187, where A005187(n) = 2n - (number of
 *          1's in binary representation of n).
 * https://oeis.org/A297111
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} (2*d - popcount(d)) * mu(n/d)
 * Range:     u8 n <= 130, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 96 terms through n = 9992 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  μ(m) is the Möbius function.  popcount(x) is the number of 1's
 *  in the binary representation of x.
 */
#include <stdint.h>
#include <stdbool.h>

/* Recursive helper to enumerate all divisors of n.
 * p   : array of prime factors of n
 * e   : corresponding exponents
 * pf  : number of distinct prime factors
 * idx : current prime index being processed
 * d   : current divisor being built
 * sq  : flag indicating whether n/d contains a squared prime
 * f1  : number of prime factors of n/d that appear to the first power
 * sum : accumulated sum of contributions
 */
static void
dfs_const(const uint64_t *p, const int *e, int pf, int idx,
          seq_t d, bool sq, int f1, long long *sum)
{
    if (idx == pf) {
        long long mu = sq ? 0 : (f1 % 2 ? -1 : 1);
        if (mu) {
            /* value (2*d - popcount(d)) fits in 32 bits */
            long long val = (long long)(2 * d - (__builtin_popcountll((unsigned long long)d)));
            *sum += mu * val;
        }
        return;
    }

    uint64_t prime = p[idx];
    int maxf = e[idx];
    seq_t pow = 1;
    for (int f = 0; f <= maxf; ++f) {
        int mexp = maxf - f;            /* exponent in (n/d) */
        bool nsq = sq;
        int nf1 = f1;
        if (mexp >= 2) nsq = true;
        else if (mexp == 1) nf1++;

        dfs_const(p, e, pf, idx + 1,
                  d * pow, nsq, nf1, sum);
        if (f < maxf) pow *= prime;
    }
}

/* Main function computing a(n). */
seq_t
a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for 0, return 0 */

    /* Factorise n (trial division, sufficient for n <= ~1e9). */
    uint64_t primes[16];
    int   exp[16];
    int   pfcnt = 0;

    uint64_t t = (uint64_t)n;
    for (uint64_t p = 2; p * p <= t; ++p) {
        if (t % p == 0) {
            int e = 0;
            while (t % p == 0) { t /= p; ++e; }
            primes[pfcnt] = p;
            exp[pfcnt]    = e;
            ++pfcnt;
        }
    }
    if (t > 1) {
        primes[pfcnt] = t;
        exp[pfcnt]    = 1;
        ++pfcnt;
    }

    long long sum = 0;  /* signed accumulator */
    dfs_const(primes, exp, pfcnt, 0, 1, false, 0, &sum);

    return (seq_t)sum;
}
