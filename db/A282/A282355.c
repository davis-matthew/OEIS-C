/*
 * A282355  Expansion of (Sum_{i>=1} x^prime(prime(i)))*(Sum_{j = p*q, p
 *          prime, q prime} x^j).
 * https://oeis.org/A282355
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 110, u16 n <= 110, u32 n <= 110, u64 n <= 110,
 *            u128 n <= 110, big any n
 * Helpers:   build_primes_upto, build_spf, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of ways of writing n as a sum of a prime with prime subscript (A006450)
 * and a semiprime (product of two primes).  The function handles any n ≥ 0
 * and is memoised for efficiency. */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* Utilities provided by the build system */
#include "../utils/is_prime_small.h"       /* bool is_prime_small(unsigned int n); */
#include "../utils/build_primes_upto.h"    /* void build_primes_upto(idx_t limit, uint64_t **p_out, size_t *cnt_out); */
#include "../utils/build_spf.h"            /* void build_spf(idx_t limit, idx_t **spf_out); */

static uint64_t *g_primes = NULL;          /* primes up to current max n */
static size_t     g_prime_cnt = 0;
static uint64_t *g_allowed = NULL;        /* primes whose 1‑based index is prime */
static size_t    g_allowed_cnt = 0;
static idx_t    *g_spf = NULL;            /* smallest prime factor for numbers up to current max */
static idx_t      g_max_n = 0;             /* maximum n for which tables are built */

static void ensure_tables(idx_t n)
{
    if (n <= g_max_n) return;                 /* tables already sufficient */

    /* free old tables */
    free(g_primes);
    free(g_allowed);
    free(g_spf);

    /* build primes up to n */
    build_primes_upto(n, &g_primes, &g_prime_cnt);

    /* build SPF array up to n */
    build_spf(n, &g_spf);

    /* build allowed primes: those whose 1‑based index is prime */
    g_allowed_cnt = 0;
    for (size_t i = 1; i <= g_prime_cnt; ++i) {
        if (is_prime_small((unsigned int)i)) g_allowed_cnt++;
    }
    if (g_allowed_cnt) {
        g_allowed = (uint64_t *)malloc(g_allowed_cnt * sizeof(uint64_t));
    } else {
        g_allowed = NULL;
    }
    size_t pos = 0;
    for (size_t i = 1; i <= g_prime_cnt; ++i) {
        if (is_prime_small((unsigned int)i)) {
            g_allowed[pos++] = g_primes[i-1];
        }
    }
    g_max_n = n;
}

/* semiprime test using the SPF array */
static bool is_semiprime(uint64_t x)
{
    if (x < 4) return false;          /* minimum semiprime is 2*2 = 4 */
    idx_t p = g_spf[x];               /* smallest prime factor (0 if x is prime) */
    if (p == 0) return false;         /* x is prime, cannot be semiprime */
    uint64_t q = x / p;
    return is_prime_small((unsigned int)q);
}

/* public function required by the problem statement */
seq_t a(idx_t n)
{
    ensure_tables(n);
    seq_t count = 0;
    for (size_t i = 0; i < g_allowed_cnt; ++i) {
        uint64_t p = g_allowed[i];
        if (p > n) break;                   /* primes are sorted ascending */
        uint64_t s = n - p;
        if (s < 4) continue;                /* semiprime must be at least 4 */
        if (is_semiprime(s)) count += 1;
    }
    return count;
}
