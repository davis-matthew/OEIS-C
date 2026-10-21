/*
 * A263020  Number of ordered pairs (k, m) with k > 0 and m > 0 such that n =
 *          pi(k*(k+1)/2) + pi(m*(3*m-1)/2), where pi(x) denotes the number of
 *          primes not exceeding x.
 * https://oeis.org/A263020
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 382, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A263020: Number of ordered pairs (k, m) with k > 0 and m > 0 such
   that n = pi(k*(k + 1)/2) + pi(m*(3*m - 1)/2).
   The function implements a direct search using pre‑computed prime
   counts up to a fixed limit and counts matches by frequency tables. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* ------------------------------------------------------------------ */
/* Parameters ---------------------------------------------------------------- */
#define PRIME_LIMIT 15000000   /* must be >= largest number needed   */

static bool init_done = false;
static uint32_t *prime_pi = NULL;    /* prime_pi[x] = pi(x), 0 <= x <= PRIME_LIMIT */

/* ------------------------------------------------------------------ */
/* One‑time initialization: build sieve and prefix prime counts       */
static void seq_initialize(void)
{
    /* allocate arrays */
    prime_pi = (uint32_t *)malloc((PRIME_LIMIT + 1) * sizeof(uint32_t));
    uint8_t *is_comp = (uint8_t *)malloc(PRIME_LIMIT + 1);
    if (!prime_pi || !is_comp) {
        exit(EXIT_FAILURE);
    }
    memset(is_comp, 0, PRIME_LIMIT + 1);

    /* Eratosthenes sieve */
    for (idx_t i = 2; i * i <= PRIME_LIMIT; ++i) {
        if (!is_comp[i]) {
            for (idx_t j = i * i; j <= PRIME_LIMIT; j += i) {
                is_comp[j] = 1;
            }
        }
    }

    /* prefix prime counts */
    uint32_t pi = 0;
    prime_pi[0] = 0;
    prime_pi[1] = 0;
    for (idx_t i = 2; i <= PRIME_LIMIT; ++i) {
        if (!is_comp[i]) ++pi;
        prime_pi[i] = pi;
    }

    free(is_comp);
    init_done = true;
}

/* ------------------------------------------------------------------ */
/* Main sequence function ------------------------------------------------ */

seq_t a(idx_t n)
{
    if (!init_done) seq_initialize();

    /* Build list of pi values for triangular numbers (k*(k+1)/2) */
    size_t tri_cap = 128, tri_len = 0;
    uint32_t *tri = (uint32_t *)malloc(tri_cap * sizeof(uint32_t));
    if (!tri) exit(EXIT_FAILURE);

    for (idx_t k = 1; ; ++k) {
        idx_t t = k * (k + 1) / 2;
        if (t > PRIME_LIMIT) break;                      /* safety guard   */
        uint32_t pi_t = prime_pi[t];
        if ((seq_t)pi_t > n) break;                     /* further values larger */
        if (tri_len == tri_cap) {
            tri_cap <<= 1;
            uint32_t *tmp = (uint32_t *)realloc(tri, tri_cap * sizeof(uint32_t));
            if (!tmp) exit(EXIT_FAILURE);
            tri = tmp;
        }
        tri[tri_len++] = pi_t;
    }

    /* Build frequency table for pentagonal pi values (m*(3*m-1)/2) */
    /* we only need frequencies up to n, so allocate array of size n+1 */
    size_t freq_size = (size_t)n + 1;
    uint32_t *freq = (uint32_t *)calloc(freq_size, sizeof(uint32_t));
    if (!freq) exit(EXIT_FAILURE);

    for (idx_t m = 1; ; ++m) {
        idx_t p = m * (3 * m - 1) / 2;
        if (p > PRIME_LIMIT) break;
        uint32_t pi_p = prime_pi[p];
        if (pi_p > n) break;
        freq[pi_p]++;                                     /* count multiplicity */
    }

    /* Count pairs whose sum equals n */
    seq_t count = 0;
    for (size_t idx = 0; idx < tri_len; ++idx) {
        seq_t remaining = (seq_t)n - (seq_t)tri[idx];
        if (remaining <= n) {
            /* remaining is guaranteed to fit in size_t because remaining <= n */
            count += (seq_t)freq[(size_t)remaining];
        }
    }

    free(tri);
    free(freq);
    return count;
}
