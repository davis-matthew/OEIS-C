/*
 * A265145  Number of lambda-parking functions of the unique strict partition
 *          lambda with parts i_1<i_2<...<i_m and encoding n =
 *          Product_{j=1..m} prime(i_j-j+1).
 * https://oeis.org/A265145
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of λ-parking functions for the strictly *
 *            increasing partition λ that is encoded by n = Π_j
 *            prime(i_j−j+1). * The function reconstructs λ from n and counts
 *            the parking * functions by dynamic programming
 * Range:     u8 n <= 31, u16 n <= 127, u32 n <= 2047, u64 n <= 9977,
 *            u128 n <= 9977, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/build_primes_upto.h"

/* small binomial coefficient for n <= 64 */
static seq_t small_binom(int n, int k)
{
    if (k < 0 || k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (int i = 1; i <= k; ++i) {
        res *= (seq_t)(n - k + i);
        res /= (seq_t)i;
    }
    return res;
}

/* integer comparison for qsort (int array) */
static int cmp_int(const void *a, const void *b)
{
    return (*(const int*)a - *(const int*)b);
}

seq_t a(idx_t n)
{
    if (n == (idx_t)1) return 1;

    /* Build list of primes up to n (n <= 10000 in the intended tests) */
    uint64_t *primes = NULL;
    size_t prime_cnt = 0;
    build_primes_upto(n, &primes, &prime_cnt);

    /* Factor n and collect prime indices (1‑based) */
    int indices[64];          /* enough for n <= 10^4 */
    size_t idx_cnt = 0;
    idx_t tmp = n;
    for (size_t pi = 0; pi < prime_cnt && tmp > 1; ++pi) {
        uint64_t p = primes[pi];
        while (tmp % p == 0) {
            tmp /= p;
            indices[idx_cnt++] = (int)(pi + 1);  /* 1‑based index */
        }
    }
    /* tmp > 1 implies it is a prime > sqrt(n) */
    if (tmp > 1) {
        /* binary search for its index in the primes array */
        size_t lo = 0, hi = prime_cnt;
        while (lo < hi) {
            size_t mid = (lo + hi) / 2;
            if (primes[mid] < tmp)
                lo = mid + 1;
            else if (primes[mid] > tmp)
                hi = mid;
            else {               /* found */
                indices[idx_cnt++] = (int)(mid + 1);
                break;
            }
        }
    }

    free(primes);              /* primes no longer needed */

    if (idx_cnt == 0) return 1;   /* n == 1 */

    /* Sort indices to form the unique λ (strict partition) */
    qsort(indices, idx_cnt, sizeof(int), cmp_int);

    /* Build λ: λ_j = indices[j] + j (j is 0‑based) */
    int *lambda = (int*)malloc(idx_cnt * sizeof(int));
    int lambda_max = 0;
    for (size_t j = 0; j < idx_cnt; ++j) {
        lambda[j] = indices[j] + (int)j;
        if (lambda[j] > lambda_max) lambda_max = lambda[j];
    }

    /* req[t] = required minimum count of elements <= t;
     * only set for t that appears in λ. 0 means no constraint. */
    int *req = (int*)calloc(lambda_max + 1, sizeof(int));
    for (size_t j = 0; j < idx_cnt; ++j)
        req[lambda[j]] = (int)(j + 1);

    /* DP: dp[r] = number of valid permutations after processing
       values up to current t having assigned r elements.          */
    seq_t prev[64] = {0}, curr[64] = {0};
    prev[0] = 1;
    size_t m = idx_cnt;

    for (int t = 1; t <= lambda_max; ++t) {
        for (size_t i = 0; i <= m; ++i) curr[i] = 0;

        for (size_t r = 0; r <= m; ++r) {
            if (prev[r] == 0) continue;
            int max_c = (int)(m - r);
            for (int c = 0; c <= max_c; ++c) {
                size_t newr = r + c;
                if (req[t] && newr < (size_t)req[t]) continue;
                seq_t term = prev[r] * small_binom((int)(r + c), c);
                curr[newr] += term;
            }
        }
        for (size_t i = 0; i <= m; ++i) prev[i] = curr[i];
    }

    seq_t result = prev[m];

    free(lambda);
    free(req);

    return result;
}
