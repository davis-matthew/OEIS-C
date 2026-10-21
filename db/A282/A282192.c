/*
 * A282192  Number of ways of writing n as a sum of a prime and a squarefree
 *          semiprime.
 * https://oeis.org/A282192
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 4049, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The OEIS sequence A282192:
 *  a(n) = number of representations of n as a prime plus a squarefree semiprime
 *  (a squarefree semiprime is a semiprime p*q with p < q distinct primes).
 *
 *  The routine computes the needed primes up to n on demand, builds a Boolean
 *  array marking the squarefree semiprimes ≤ n, then counts for each prime r≤n
 *  whether s = n – r is a marked squarefree semiprime.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* forward declarations of static helpers */
static void compute_primes(idx_t limit, idx_t **primes_out, size_t *cnt_out);
static void rebuild_sfs(idx_t limit);

/* static cached data shared across calls */
static idx_t    g_limit      = 0;
static idx_t   *g_primes     = NULL;   /* array of primes up to g_limit */
static size_t    g_prime_cnt = 0;
static uint8_t  *g_sfs_flag  = NULL;   /* non‑zero iff i is a squarefree semiprime */

/* ------------------------------------------------------------ */
static void
compute_primes(idx_t limit, idx_t **primes_out, size_t *cnt_out)
{
    /* Simple sieve of Eratosthenes up to 'limit' */
    uint8_t *is_prime = (uint8_t *)calloc(limit + 1, sizeof(uint8_t));
    size_t   i, j;
    if (!is_prime) abort();

    /* 0 and 1 are not prime */
    if (limit >= 0) is_prime[0] = 0;
    if (limit >= 1) is_prime[1] = 0;
    for (i = 2; i <= limit; ++i) is_prime[i] = 1;

    for (i = 2; i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (j = i * i; j <= limit; j += i)
                is_prime[j] = 0;
        }
    }

    /* count primes */
    size_t prime_cnt = 0;
    for (i = 2; i <= limit; ++i) if (is_prime[i]) ++prime_cnt;

    /* allocate array and fill */
    idx_t *primes = (idx_t *)malloc(prime_cnt * sizeof(idx_t));
    if (!primes) abort();
    size_t idx = 0;
    for (i = 2; i <= limit; ++i) if (is_prime[i]) primes[idx++] = i;

    *primes_out = primes;
    *cnt_out    = prime_cnt;

    free(is_prime);
}

/* rebuild the squarefree semiprime flags up to 'limit' */
static void
rebuild_sfs(idx_t limit)
{
    /* free old flag array */
    free(g_sfs_flag);
    g_sfs_flag = (uint8_t *)calloc(limit + 1, sizeof(uint8_t));
    if (!g_sfs_flag) abort();

    /* cross out products of distinct primes <= limit */
    for (size_t i = 0; i < g_prime_cnt; ++i) {
        for (size_t j = i + 1; j < g_prime_cnt; ++j) {
            idx_t prod = g_primes[i] * g_primes[j];
            if (prod > limit) break;
            g_sfs_flag[prod] = 1;
        }
    }
}

/* ensure that all tables are built up to at least 'n' */
static void
ensure_memo(idx_t n)
{
    if (n <= g_limit) return;

    /* free existing primes array */
    free(g_primes);
    g_primes = NULL;

    /* build new primes up to n */
    compute_primes(n, &g_primes, &g_prime_cnt);

    /* rebuild sfs flags */
    rebuild_sfs(n);

    g_limit = n;
}

/* ------------------------------------------------------------ */
seq_t
a(idx_t n)
{
    /* make sure tables cover 'n' */
    ensure_memo(n);

    seq_t count = 0;
    for (size_t pi = 0; pi < g_prime_cnt; ++pi) {
        idx_t p = g_primes[pi];
        if (p > n) break;
        idx_t s = n - p;
        if (s >= 2 && g_sfs_flag[s]) {
            ++count;
        }
    }
    return count;
}
