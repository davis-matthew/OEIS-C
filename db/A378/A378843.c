/*
 * A378843  Number of compositions (ordered partitions) of n into distinct
 *          squarefree divisors of n.
 * https://oeis.org/A378843
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 59, u16 n <= 209, u32 n <= 419, u64 n <= 2309,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function computes a(n) = number of compositions of n into distinct squarefree divisors of n.
   It uses dynamic programming over subsets of the squarefree divisors of n. */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* a(0) = 1 (empty composition) */
    if (n == 0)
        return (seq_t)1;

    /* --------------------------------------------------------------------
       1. Find distinct prime factors of n
       -------------------------------------------------------------------- */
    uint64_t primes[10];
    size_t pcnt = 0;
    uint64_t tmp = n;
    for (uint64_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            primes[pcnt++] = p;
            while (tmp % p == 0)
                tmp /= p;
        }
    }
    if (tmp > 1)
        primes[pcnt++] = tmp;

    /* --------------------------------------------------------------------
       2. Generate all squarefree divisors of n
          (products of subsets of the distinct primes)
       -------------------------------------------------------------------- */
    size_t m = (size_t)1 << pcnt;               /* number of squarefree divisors   */
    uint64_t *sdiv = (uint64_t *)malloc(m * sizeof(uint64_t));
    if (!sdiv) return (seq_t)0;                 /* allocation failure (unlikely)  */
    for (size_t mask = 0; mask < m; ++mask) {
        uint64_t prod = 1;
        for (size_t i = 0; i < pcnt; ++i)
            if (mask & ((size_t)1 << i))
                prod *= primes[i];
        sdiv[mask] = prod;
    }

    /* --------------------------------------------------------------------
       3. DP table: dp[s * (m+1) + k] = number of subsets of size k
          whose elements sum to s.
       -------------------------------------------------------------------- */
    size_t kmax = m;
    size_t dp_cols = kmax + 1;
    size_t dp_rows = (size_t)n + 1;
    seq_t *dp = (seq_t *)calloc(dp_rows * dp_cols, sizeof(seq_t));
    if (!dp) { free(sdiv); return (seq_t)0; }   /* allocation failure          */
    dp[0 * dp_cols + 0] = (seq_t)1;             /* empty set for sum 0 */

    /* --------------------------------------------------------------------
       4. Populate DP.  For each squarefree divisor d, update dp
          in reverse order to ensure each divisor is used at most once.
       -------------------------------------------------------------------- */
    for (size_t idx = 0; idx < m; ++idx) {
        uint64_t d = sdiv[idx];
        if (d == 0 || d > n) continue;
        size_t max_s = (size_t)n - d;
        for (size_t s = max_s + 1; s-- > 0; ) {    /* s = 0 .. max_s            */
            for (size_t k = kmax; k-- > 0; ) {     /* k = kmax-1 .. 0          */
                seq_t ways = dp[s * dp_cols + k];
                if (ways)
                    dp[(s + d) * dp_cols + (k + 1)] += ways;
            }
        }
    }

    /* --------------------------------------------------------------------
       5. Precompute factorials up to kmax
       -------------------------------------------------------------------- */
    static seq_t fact[33];
    fact[0] = (seq_t)1;
    for (size_t i = 1; i <= kmax; ++i)
        fact[i] = fact[i - 1] * (seq_t)i;

    /* --------------------------------------------------------------------
       6. Sum over all subsets of size k that reach sum n, weighted by k!
       -------------------------------------------------------------------- */
    seq_t result = (seq_t)0;
    size_t base = (size_t)n * dp_cols;
    for (size_t k = 1; k <= kmax; ++k) {
        seq_t cnt = dp[base + k];
        if (cnt)
            result += cnt * fact[k];
    }

    free(sdiv);
    free(dp);
    return result;
}
