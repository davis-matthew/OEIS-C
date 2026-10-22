/*
 * A368710  The maximal exponent in the prime factorization of the powerful
 *          numbers.
 * https://oeis.org/A368710
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A051903(A001694(n))
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the maximal exponent in the prime factorisation of the n-th
   powerful number.  Sequence is memoised and regenerated if the requested
   index exceeds the currently pre‑computed range.  */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"
#include "../utils/factor_u64.h"

/* ----- helpers -------------------------------------------------------- */
static uint64_t floor_sqrt(uint64_t n) {
    uint64_t x = n, y = (x + 1) >> 1;
    while (y < x) {
        x = y;
        y = (x + n / x) >> 1;
    }
    return x;
}

static int cmp_uint64(const void *a, const void *b) {
    uint64_t va = *(const uint64_t *)a;
    uint64_t vb = *(const uint64_t *)b;
    if (va < vb) return -1;
    if (va > vb) return 1;
    return 0;
}

/* ----- global storage ----------------------------------------------- */
static uint64_t *g_primes    = NULL;      /* primes up to sqrt(limit)   */
static size_t    g_prime_cnt = 0;
static uint64_t *g_powers    = NULL;      /* powerful numbers (sorted)  */
static size_t    g_power_cnt = 0;
static size_t    g_power_cap = 0;
static uint64_t  g_limit     = 50000000ULL;   /* initial bound */

/* ----- recursive generator ------------------------------------------- */
static void gen_powers(idx_t start, uint64_t prod, uint64_t limit) {
    for (idx_t i = start; i < g_prime_cnt; ++i) {
        uint64_t p   = g_primes[i];
        uint64_t p2  = p * p;
        if (p2 > limit) break;
        uint64_t pow = p2;
        while (1) {
            if (prod > limit / pow) break;
            uint64_t newprod = prod * pow;
            if (g_power_cnt >= g_power_cap) {
                g_power_cap = g_power_cap ? g_power_cap * 2 : 16;
                uint64_t *tmp = (uint64_t *)realloc(g_powers, g_power_cap * sizeof(uint64_t));
                g_powers = tmp;
            }
            g_powers[g_power_cnt++] = newprod;
            gen_powers(i + 1, newprod, limit);
            if (pow > limit / p) break;
            pow *= p;
        }
    }
}

/* ----- regenerate tables when more numbers are needed ----------------- */
static void regenerate(uint64_t limit) {
    /* free old data */
    if (g_primes) free(g_primes);
    if (g_powers) free(g_powers);

    /* new prime list */
    uint64_t sqrtlim = floor_sqrt(limit);
    uint64_t *tmp_primes = NULL;
    size_t   tmp_cnt = 0;
    build_primes_upto(sqrtlim, &tmp_primes, &tmp_cnt);
    g_primes    = tmp_primes;
    g_prime_cnt = tmp_cnt;

    /* allocate powers buffer */
    g_power_cap = 1024;
    g_powers    = (uint64_t *)malloc(g_power_cap * sizeof(uint64_t));
    g_power_cnt = 0;
    g_powers[g_power_cnt++] = 1ULL;   /* 1 is always powerful */

    /* generate all powerful numbers up to `limit` */
    gen_powers(0, 1ULL, limit);

    /* sort the list */
    qsort(g_powers, g_power_cnt, sizeof(uint64_t), cmp_uint64);

    g_limit = limit;
}

/* ----- max exponent of a powerful number ----------------------------- */
static seq_t max_exponent(uint64_t v) {
    uint64_t pr[64];
    uint64_t ex[64];
    size_t   cnt = 0;
    factor_u64(v, pr, ex, &cnt);
    seq_t maxexp = 0;
    for (size_t i = 0; i < cnt; ++i)
        if (ex[i] > maxexp) maxexp = ex[i];
    return maxexp;
}

/* ----- public sequence function -------------------------------------- */
seq_t a(idx_t n) {
    if (n == (idx_t)1) return (seq_t)0;       /* a(1) = 0 by definition */

    /* initialise or grow tables to cover requested index */
    if (g_powers == NULL) regenerate(g_limit);
    while ((size_t)n > g_power_cnt) {
        g_limit *= 2;
        regenerate(g_limit);
    }

    uint64_t powerful = g_powers[(size_t)n - 1];
    return max_exponent(powerful);
}
