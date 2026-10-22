/*
 * A390913  Largest positive integer k such that exactly 1/n of the integers
 *          from 1 to k are divisors of k, or 0 if no solution exists.
 * https://oeis.org/A390913
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = max{ k ∈ ℕ : d(k) * n = k } (OEIS A390913)
 * Range:     u8 n <= 14, u16 n <= 629, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "../utils/gcd.h"            // for seq_t types etc. (provided by build)

/* ------------------------------------------------------------------
   Smallest prime factor table (dynamic, built on demand)
   ------------------------------------------------------------------ */
static uint32_t *spf = NULL;
static uint32_t spf_limit = 0;

/* build or extend the SPF table up to 'limit' inclusive */
static void spf_sieve(uint32_t limit)
{
    if (limit <= spf_limit) return;                 /* already built enough */
    uint32_t new_limit = spf_limit > 0 ? spf_limit : limit;
    if (new_limit < limit) new_limit = limit;
    uint32_t *new_spf = (uint32_t*)realloc(spf, (new_limit + 1) * sizeof(uint32_t));
    if (!new_spf) { spf = NULL; spf_limit = 0; return; }
    spf = new_spf;
    for (uint32_t i = spf_limit + 1; i <= new_limit; ++i) spf[i] = i;
    for (uint32_t i = 2; i * i <= new_limit; ++i)
        if (spf[i] == i)
            for (uint32_t j = i * i; j <= new_limit; j += i)
                if (spf[j] == j) spf[j] = i;
    spf_limit = new_limit;
}

/* ------------------------------------------------------------------
   Factorisation helpers
   ------------------------------------------------------------------ */
#define MAX_FACT 64   /* enough for all m <= 4*10^4 */

typedef struct {
    uint32_t pr[MAX_FACT];
    uint32_t exp[MAX_FACT];
    uint32_t cnt;
} ftable_t;

/* factorise 'x' using the global SPF table into 'ft' */
static void factor_uint32(uint32_t x, ftable_t *ft)
{
    ft->cnt = 0;
    while (x > 1) {
        uint32_t p = spf[x];
        uint32_t e = 0;
        while (x % p == 0) { x /= p; ++e; }
        ft->pr[ft->cnt] = p;
        ft->exp[ft->cnt] = e;
        ++ft->cnt;
    }
}

/* combine divisor counts of two numbers given their factorizations */
static seq_t divisor_count_product(const ftable_t *fa, const ftable_t *fb)
{
    seq_t res = 1;
    uint32_t i = 0, j = 0;
    while (i < fa->cnt && j < fb->cnt) {
        if (fa->pr[i] < fb->pr[j])         { res *= (seq_t)(fa->exp[i] + 1); ++i; }
        else if (fa->pr[i] > fb->pr[j])    { res *= (seq_t)(fb->exp[j] + 1); ++j; }
        else { /* equal prime */           res *= (seq_t)(fa->exp[i] + fb->exp[j] + 1); ++i; ++j; }
    }
    while (i < fa->cnt) { res *= (seq_t)(fa->exp[i] + 1); ++i; }
    while (j < fb->cnt) { res *= (seq_t)(fb->exp[j] + 1); ++j; }
    return res;
}

/* ------------------------------------------------------------------
   Main sequence function
   ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                     /* undefined for n < 1, return 0 */

    /* maximum m to test: m <= 4*n (k <= 4*n^2) */
    uint32_t u_n = (uint32_t)n;
    uint32_t max_m = 4 * u_n;
    uint32_t limit = (max_m > u_n) ? max_m : u_n;   /* ensure SPF covers m */

    spf_sieve(limit);                       /* build SPF up to max needed */

    /* factorise n */
    ftable_t f_n;
    factor_uint32(u_n, &f_n);

    seq_t best_k = 0;

    ftable_t f_m;
    for (uint32_t m = 1; m <= max_m; ++m) {
        factor_uint32(m, &f_m);
        seq_t d_prod = divisor_count_product(&f_n, &f_m);
        if (d_prod == (seq_t)m) {
            seq_t k = (seq_t)u_n * (seq_t)m;
            if (k > best_k) best_k = k;
        }
    }
    return best_k;
}
