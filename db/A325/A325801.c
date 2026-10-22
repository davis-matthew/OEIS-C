/*
 * A325801  Number of divisors of n minus the number of distinct positive
 *          subset-sums of the prime indices of n.
 * https://oeis.org/A325801
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of divisors of n – number of distinct positive
 *            subset-sums (including the empty subset) of the multiset of
 *            prime indices of n. For each prime factor p^e of n we include
 *            the index of p exactly e times. Since n ≤ 10000 the multisets
 *            are very small and a simple DP suffices
 * Range:     u8 n <= 9765, u16 n <= 9765, u32 n <= 9765, u64 n <= 9765,
 *            u128 n <= 9765, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

static bool _a325801_inited = false;
static int   *_a325801_primes   = NULL;
static size_t _a325801_prime_cnt = 0;
static int   *_a325801_prime_idx = NULL;  /* index (1‑based) of each prime ≤10000 */

static void _a325801_init(void)
{
    const int MAX_N = 10000;
    int i, j, is_prime[MAX_N + 1];
    for (i = 0; i <= MAX_N; ++i) is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;
    for (i = 2; i * i <= MAX_N; ++i)
        if (is_prime[i])
            for (j = i * i; j <= MAX_N; j += i)
                is_prime[j] = 0;

    /* Count primes and collect them */
    for (i = 2, _a325801_prime_cnt = 0; i <= MAX_N; ++i)
        if (is_prime[i]) ++_a325801_prime_cnt;
    _a325801_primes = (int *)malloc(_a325801_prime_cnt * sizeof(int));
    _a325801_prime_idx = (int *)malloc((MAX_N + 1) * sizeof(int));
    memset(_a325801_prime_idx, -1, (MAX_N + 1) * sizeof(int));
    for (i = 2, j = 0; i <= MAX_N; ++i)
        if (is_prime[i]) {
            _a325801_primes[j] = i;
            _a325801_prime_idx[i] = j + 1;  /* 1‑based index */
            ++j;
        }
    _a325801_inited = true;
}

seq_t a(idx_t n)
{
    /* Ensure initialisation (once) */
    if (!_a325801_inited) _a325801_init();

    /* Factorisation, building the multiset of prime indices (with multiplicity)
       and computing the divisor count tau */
    seq_t tau = 1;
    int  indices[64];   /* ample for n ≤ 10000 (max exponents sum < 14) */
    int  idx_len = 0;

    idx_t m = n;
    for (size_t pi = 0; pi < _a325801_prime_cnt &&
                       (idx_t)(_a325801_primes[pi]) * (idx_t)(_a325801_primes[pi]) <= m; ++pi) {
        int p = _a325801_primes[pi];
        if (m % (idx_t)p == 0) {
            int e = 0;
            while (m % (idx_t)p == 0) {
                m /= (idx_t)p;
                ++e;
            }
            tau = tau * (seq_t)(e + 1);
            int idx = _a325801_prime_idx[p];
            for (int k = 0; k < e; ++k) {
                indices[idx_len++] = idx;
            }
        }
    }
    if (m > 1) {  /* remaining prime factor > sqrt(n) */
        int p = (int)m;
        tau = tau * (seq_t)2;  /* exponent 1 */
        int idx = _a325801_prime_idx[p];
        indices[idx_len++] = idx;
    }

    /* Compute all distinct subset sums (including 0) via DP */
    int total_sum = 0;
    for (int i = 0; i < idx_len; ++i) total_sum += indices[i];

    unsigned char *dp = (unsigned char *)calloc((size_t)total_sum + 1, 1);
    if (!dp) return (seq_t)0;  /* fallback; should not happen */
    dp[0] = 1;
    for (int i = 0; i < idx_len; ++i) {
        int v = indices[i];
        for (int s = total_sum; s >= v; --s)
            if (dp[s - v]) dp[s] = 1;
    }

    /* Count the number of true entries (= distinct sums) */
    int count = 0;
    for (int s = 0; s <= total_sum; ++s)
        if (dp[s]) ++count;

    free(dp);

    /* Result: tau - count */
    seq_t result = tau - (seq_t)count;
    return result;
}
