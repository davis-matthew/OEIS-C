/*
 * A280534  Number of partitions of n into two parts with the smaller part
 *          prime and the larger part squarefree.
 * https://oeis.org/A280534
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{p prime≤⌊n/2⌋} μ(n–p)² * Implementation: count primes
 *            p up to ⌊n/2⌋ for which n‑p is square‑free * (i.e. μ(n‑p) ≠ 0).
 *            μ and the prime list are built once up to the * largest n
 *            requested; subsequent calls reuse the cached data
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Cached state */
static idx_t  max_precalc = 0;          /* largest n for which tables are built */
static int8_t *mu_tbl      = NULL;      /* μ[0..max_precalc] */
static uint32_t *primes_tbl = NULL;     /* list of primes up to max_precalc */
static size_t   primes_used = 0;        /* number of primes stored */

/* Build μ and prime list up to N (inclusive) using the linear sieve. */
static void build_mu_and_primes(idx_t N)
{
    if (N <= max_precalc)
        return;

    /* Resize tables as needed. */
    mu_tbl      = (int8_t *)realloc(mu_tbl, (size_t)(N + 1) * sizeof(int8_t));
    if (!mu_tbl) exit(EXIT_FAILURE);
    primes_tbl  = (uint32_t *)realloc(primes_tbl, (size_t)(N + 1) * sizeof(uint32_t));
    if (!primes_tbl) exit(EXIT_FAILURE);

    /* Arrays are freshly allocated; initialise μ[0] and μ[1]. */
    mu_tbl[0] = 0;
    mu_tbl[1] = 1;

    /* Boolean composite flags. */
    uint8_t *is_comp = (uint8_t *)calloc((size_t)(N + 1), 1);
    if (!is_comp) exit(EXIT_FAILURE);

    size_t pc = 0;   /* count of primes found */

    for (idx_t i = 2; i <= N; ++i) {
        if (!is_comp[i]) {
            primes_tbl[pc++] = (uint32_t)i;
            mu_tbl[i] = -1;
        }
        for (size_t j = 0; j < pc; ++j) {
            idx_t p = primes_tbl[j];
            idx_t v = i * p;
            if (v > N) break;
            is_comp[v] = 1;
            if (i % p == 0) {
                mu_tbl[v] = 0;
                break;
            } else {
                mu_tbl[v] = -mu_tbl[i];
            }
        }
    }

    free(is_comp);
    max_precalc = N;
    primes_used = pc;
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;  /* offset 1: a(1) is the first term */

    /* Ensure tables cover this n. */
    build_mu_and_primes(n);

    idx_t limit = n / 2;
    seq_t count = 0;

    for (size_t i = 0; i < primes_used && primes_tbl[i] <= limit; ++i) {
        idx_t p = (idx_t)primes_tbl[i];
        idx_t m = n - p;
        if (mu_tbl[(size_t)m] != 0)
            ++count;
    }

    return count;
}
