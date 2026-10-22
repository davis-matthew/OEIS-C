/*
 * A334655  Number of integers less than n with the same number of distinct
 *          prime factors as n.
 * https://oeis.org/A334655
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |{j < n : ω(j) = ω(n)}| where ω(k) is the number of
 *            distinct prime factors of k
 * Range:     u8 n <= 481, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *freq_cache = NULL;          /* freq_cache[ω] = count of numbers ≤ current limit with ω distinct primes */
static size_t freq_len   = 0;
static idx_t  precomp_lim = 0;            /* largest n for which data is prepared (inclusive) */

static uint64_t *omega_cache = NULL;     /* omega_cache[i] = ω(i) for 0 ≤ i ≤ precomp_lim */
static size_t   omega_len   = 0;

/* Ensure all data are computed for n (inclusive).  Calls are idempotent. */
static void ensure_precomputed(idx_t n)
{
    if (n <= precomp_lim) return;        /* already up to date */

    idx_t limit = n;                     /* include n itself */

    /* ---------- generate primes up to limit ---------- */
    idx_t    sz = limit + 1;
    char *is_comp = (char *)calloc(sz, sizeof(char)); /* 0 = prime, 1 = composite */

    for (idx_t i = 2; i * i <= limit; ++i)
        if (!is_comp[i])
            for (idx_t j = i * i; j <= limit; j += i)
                is_comp[j] = 1;

    /* count primes to allocate array */
    idx_t primes_cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (!is_comp[i]) ++primes_cnt;

    uint64_t *primes = (uint64_t *)malloc(primes_cnt * sizeof(uint64_t));
    idx_t k = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (!is_comp[i]) primes[k++] = i;

    free(is_comp);

    /* ---------- compute ω(i) for i = 0..limit ---------- */
    uint64_t *omega = (uint64_t *)calloc(limit + 1, sizeof(uint64_t)); /* zero-initialised */

    for (idx_t pi = 0; pi < primes_cnt; ++pi) {
        uint64_t p = primes[pi];
        for (idx_t j = p; j <= limit; j += p)
            ++omega[j];
    }

    free(primes);

    /* ---------- determine maximum ω to size freq array ---------- */
    uint64_t max_omega = 0;
    for (idx_t i = 1; i <= limit; ++i)
        if (omega[i] > max_omega) max_omega = omega[i];

    /* ---------- build freq array ---------- */
    seq_t *new_freq = (seq_t *)calloc(max_omega + 1, sizeof(seq_t));
    for (idx_t i = 1; i <= limit; ++i)
        ++new_freq[omega[i]];

    /* ---------- release old caches and store new ones ---------- */
    free(freq_cache);
    freq_cache = new_freq;
    freq_len   = max_omega + 1;

    free(omega_cache);
    omega_cache = omega;
    omega_len   = limit + 1;

    precomp_lim = limit;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined but guard against zero */
    ensure_precomputed(n);                 /* compute up to n (inclusive) */

    uint64_t og = omega_cache[n];          /* ω(n) */
    /* freq_cache[og] counts numbers ≤ n having ω = og, so subtract 1 for j < n */
    return freq_cache[og] - 1;
}
