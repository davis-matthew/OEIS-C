/*
 * A331773  a(n) = Sum_{-n<i<n, -n<j<n, gcd{i,j}=2} (n-|i|)*(n-|j|)/8.
 * https://oeis.org/A331773
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ( n*(n-2) + Σ_{d=1..⌊(n-1)/2⌋} μ(d) * ( n*⌊⌊(n-1)/2⌋/d⌋ -
 *            d*⌊⌊(n-1)/2⌋/d⌋*(⌊⌊(n-1)/2⌋/d⌋+1) )² ) / 2
 * Range:     u8 n <= 8, u16 n <= 36, u32 n <= 613, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements sequence A331773.  It uses a precomputed table of Möbius
   function values up to 5000 (enough for n ≤ 10000).  All arithmetic
   is performed with the arbitrary‑precision type seq_t.  For n ≤ 2 the
   result is defined to be 0. */

#include <stdint.h>
#include <stdlib.h>

/* Maximum M = floor((n-1)/2) for n≤10000 → Mmax = 4999.  Allocate one
   element beyond for 1‑based indexing. */
#define MU_MAX 5000
static int8_t mu[MU_MAX + 1];
static int mu_computed = 0;

/* Linear sieve to compute Möbius values up to MU_MAX. */
static void
compute_mu(void)
{
    if (mu_computed) return;
    int i, p;
    int *primes = (int *)malloc(sizeof(int) * (MU_MAX + 1));
    int primes_cnt = 0;
    int *lp = (int *)malloc(sizeof(int) * (MU_MAX + 1));
    for (i = 0; i <= MU_MAX; ++i) {
        mu[i] = 0;
        lp[i] = 0;
    }
    mu[1] = 1;
    for (i = 2; i <= MU_MAX; ++i) {
        if (lp[i] == 0) {
            lp[i] = i;
            mu[i] = -1;
            primes[primes_cnt++] = i;
        }
        for (p = 0; p < primes_cnt; ++p) {
            int pr = primes[p];
            if (pr > lp[i]) break;
            int ip = i * pr;
            if (ip > MU_MAX) break;
            lp[ip] = pr;
            if (pr == lp[i]) {
                mu[ip] = 0;
                break;
            } else {
                mu[ip] = -mu[i];
            }
        }
    }
    free(primes);
    free(lp);
    mu_computed = 1;
}

/* Main function implementing the sequence. */
seq_t
a(idx_t n)
{
    if (n <= 2) return 0;   /* a(1) = a(2) = 0 */
    compute_mu();           /* ensure μ values up to MU_MAX are ready */

    /* M = floor((n-1)/2) */
    idx_t M = (n - 1) / 2;

    /* term1 = n*(n-2) */
    seq_t term1 = (seq_t)n * ((seq_t)n - 2U);

    /* Compute the Möbius sum S = Σ μ(d) * F(d)² where
       F(d) = n*⌊M/d⌋ - d*⌊M/d⌋*(⌊M/d⌋+1)      */
    seq_t posSum = 0;   /* sum of terms with μ(d)=+1 */
    seq_t negSum = 0;   /* sum of terms with μ(d)=-1 */

    for (idx_t d = 1; d <= M; ++d) {
        int8_t mu_d = mu[d];
        if (mu_d == 0) continue;
        idx_t s = M / d;                       /* S1 */
        seq_t nS = (seq_t)n * (seq_t)s;
        seq_t sub = (seq_t)d * (seq_t)s * ((seq_t)s + 1U);
        seq_t val = nS - sub;                 /* non‑negative for n≥3 */
        seq_t sq = val * val;
        if (mu_d > 0) {
            posSum += sq;
        } else {
            negSum += sq;
        }
    }
    seq_t S = posSum - negSum;   /* μ‑inverted sum */

    /* a(n) = (term1 + S) / 2 */
    seq_t numerator = term1 + S;
    seq_t result = numerator / 2U;
    return result;
}
