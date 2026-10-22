/*
 * A368116  A(m, n) = lcm_{p in Partitions(n)} (Product_{r in p}(r + m)).
 *          Array read by ascending antidiagonals, for m, n >= 0.
 * https://oeis.org/A368116
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = A(m,n) where A(m,n) = lcm_{p ∈ P(n)} ∏_{r∈p} (r+m), with
 *            (m,n) obtained from the index n by traversing the integer
 *            antidiagonals:
 *            (m=0,n=0),(m=0,n=1),(m=1,n=0),(m=0,n=2),(m=1,n=1),(m=2,n=0),…
 * Range:     u8 n <= 17, u16 n <= 31, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   prime_val_in, seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function performs the following steps for each query: 1) map the
   index to the pair (m,k) on an antidiagonal; 2) for each prime p
   ≤ (m+k) compute the maximal exponent of p in a product over a partition
   of k by an unbounded‑knapsack DP; 3) multiply all prime powers to
   obtain the lcm.  The algorithm is O(k²·π(m+k)) per call; for the
   index range used in the tests k ≤ ≈ 150 and π(≤150)≈33, which is
   comfortably fast.  All arithmetic is performed in seq_t, with
   seq_mul and seq_pow taken from the utilities. */

#include <stdint.h>
#include <stdbool.h>
#include <math.h>

/* Utility functions from the build environment */
#include "../utils/seq_mul.h"   /* seq_mul(seq_t a, seq_t b) */
#include "../utils/seq_pow.h"   /* seq_pow(seq_t base, idx_t exp) */
#include "../utils/prime_val_in.h"

/* ------------------------------------------------------------ */
/* Prime sieve – static, executed once                           */
#define PRIME_MAX 2000
static uint64_t _primes[PRIME_MAX];
static size_t _prime_cnt = 0;
static bool _primes_inited = false;

static void init_primes(void)
{
    if (_primes_inited)
        return;
    bool is_prime[PRIME_MAX + 1];
    for (int i = 0; i <= PRIME_MAX; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (int i = 2; i * i <= PRIME_MAX; ++i) {
        if (is_prime[i]) {
            for (int j = i * i; j <= PRIME_MAX; j += i)
                is_prime[j] = false;
        }
    }
    for (int i = 2; i <= PRIME_MAX; ++i)
        if (is_prime[i]) {
            _primes[_prime_cnt++] = (uint64_t)i;
        }
    _primes_inited = true;
}

/* ------------------------------------------------------------ */

/* ------------------------------------------------------------ */
/* Main sequence function */
seq_t a(idx_t n)
{
    /* 1. Map the linear index to coordinates (m,k) on an antidiagonal */
    if (n == 0)
        return (seq_t)1;   /* A(0,0) */

    /* binary search or double sqrt to find diagonal d:
       d*(d+1)/2 ≤ n < (d+1)*(d+2)/2                              */
    double d_real = floor((sqrt(8.0 * (double)n + 1.0) - 1.0) / 2.0);
    idx_t d = (idx_t)d_real;
    /* correct possible rounding errors */
    while ((d + 1) * (d + 2) / 2 <= n) ++d;
    while (d * (d + 1) / 2 > n) --d;
    idx_t start = d * (d + 1) / 2;
    idx_t k = n - start;            /* offset within the diagonal        */
    idx_t m = d - k;                /* coordinates m and n'=k             */

    if (k == 0)
        return (seq_t)1;   /* A(m,0) = 1 */

    /* 2. Prepare primes up to m + k */
    init_primes();
    idx_t limit = m + k;                /* maximum value of (m+r)            */

    /* 3. Dynamic programming to obtain maximal exponent for each prime */
    static int val[PRIME_MAX + 1];       /* v_p(m+r) for r = 1..k            */
    static int dp[PRIME_MAX + 1];        /* dp[w] – maximum exponent for weight w */
    seq_t result = (seq_t)1;             /* the final lcm                     */

    for (size_t pi = 0; pi < _prime_cnt && _primes[pi] <= limit; ++pi) {
        unsigned long long p = _primes[pi];

        /* Precompute v_p for all parts */
        for (idx_t r = 1; r <= k; ++r) {
            unsigned long long val_r = m + r;
            val[(int)r] = prime_val_in(val_r, p);
        }

        /* DP initialisation */
        for (idx_t w = 0; w <= k; ++w)
            dp[w] = -1;
        dp[0] = 0;

        /* Unbounded‑knapsack DP */
        for (idx_t w = 1; w <= k; ++w) {
            int best = -1;
            for (idx_t r = 1; r <= w; ++r) {
                if (dp[w - r] >= 0) {
                    int cand = dp[w - r] + val[(int)r];
                    if (cand > best)
                        best = cand;
                }
            }
            dp[w] = best;
        }

        int exp = dp[(int)k];
        if (exp > 0) {
            seq_t powp = seq_pow((seq_t)p, (idx_t)exp);
            result = seq_mul(result, powp);
        }
    }

    return result;
}
