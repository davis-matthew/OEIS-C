/*
 * A352493  Number of non-constant integer partitions of n into prime parts
 *          with prime multiplicities.
 * https://oeis.org/A352493
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Partition count: a(n) = number of ways to write n as
   a sum of distinct prime parts p_i, each used m_i times,
   where both p_i and m_i are prime, and at least two distinct
   primes are used. */
#include <stddef.h>
#include <stdlib.h>

/* External integer types defined by the build system */
typedef uint64_t idx_t;

/* The sequence value type; defined by the build system */
extern seq_t a(idx_t n);

/* ---------- Helper: generate primes up to a limit ---------- */
static void generate_primes(idx_t limit, uint64_t **primes_out, size_t *cnt_out)
{
    if (limit < 2) {
        *primes_out = NULL;
        *cnt_out    = 0;
        return;
    }
    /* Boolean sieve */
    size_t sz = (size_t)(limit + 1);
    char *is_prime = (char*)calloc(sz, sizeof(char));
    for (size_t i = 2; i < sz; ++i) is_prime[i] = 1;
    for (size_t p = 2; p * p <= (size_t)limit; ++p) {
        if (is_prime[p]) {
            for (size_t j = p * p; j < sz; j += p) is_prime[j] = 0;
        }
    }
    /* Count primes */
    size_t cnt = 0;
    for (size_t i = 2; i < sz; ++i) if (is_prime[i]) ++cnt;
    uint64_t *primes = (uint64_t*)malloc(cnt * sizeof(uint64_t));
    size_t idx = 0;
    for (size_t i = 2; i < sz; ++i) if (is_prime[i]) primes[idx++] = i;
    *primes_out = primes;
    *cnt_out    = cnt;
    free(is_prime);
}

/* ---------- Helper: binary search in the prime list ---------- */
static int is_prime_in_list(uint64_t x, const uint64_t *primes, size_t cnt)
{
    size_t lo = 0, hi = cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (primes[mid] == x) return 1;
        if (primes[mid] < x) lo = mid + 1;
        else hi = mid;
    }
    return 0;
}

/* ---------- Main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* Generate all primes up to n */
    uint64_t *primes = NULL;
    size_t prime_cnt = 0;
    generate_primes(n, &primes, &prime_cnt);

    size_t N = (size_t)n;

    /* Allocate DP arrays */
    seq_t *dp    = (seq_t*)calloc(N + 1, sizeof(seq_t));
    seq_t *next  = (seq_t*)calloc(N + 1, sizeof(seq_t));
    if (!dp || !next) {
        free(primes);
        return 0; /* allocation failure */
    }
    dp[0] = 1;

    /* Outer loop over primes (each group contains one prime part) */
    for (size_t pi = 0; pi < prime_cnt; ++pi) {
        uint64_t p = primes[pi];

        /* Copy current dp into next (representing the option of not using p) */
        for (size_t i = 0; i <= N; ++i) next[i] = dp[i];

        /* Consider all prime multiplicities m for this prime p */
        for (size_t mj = 0; mj < prime_cnt; ++mj) {
            uint64_t m = primes[mj];
            uint64_t w = p * m;
            if (w > n) break;            /* larger m will only increase w */
            size_t wsize = (size_t)w;     /* safe because w <= n */
            for (size_t s = wsize; s <= N; ++s) {
                next[s] += dp[s - wsize];
            }
        }

        /* Prepare for next prime */
        seq_t *tmp = dp;
        dp        = next;
        next      = tmp;
    }

    seq_t result = dp[N];

    /* Subtract partitions that use only a single prime part */
    seq_t single = 0;
    for (size_t pi = 0; pi < prime_cnt; ++pi) {
        uint64_t p = primes[pi];
        if (n % p == 0) {
            uint64_t m = n / p;
            if (is_prime_in_list(m, primes, prime_cnt)) {
                ++single;
            }
        }
    }
    result -= single;

    /* Clean up */
    free(primes);
    free(dp);
    free(next);
    return result;
}
