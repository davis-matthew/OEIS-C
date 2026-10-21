/*
 * A253642  Number of ways the perfect power A001597(n) can be written as a^b,
 *          with a, b > 1.
 * https://oeis.org/A253642
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A253642: Number of ways the nth perfect power can be written as a^b (a,b>1)
   Formula: a(n) = τ(gcd(exponents of prime factorization of A001597(n))) – 1
   where A001597(n) is the nth perfect power (1,4,8,9,16,25,…).  The term
   a(1)=0 corresponds to 1 having no representation with a>1.  The code
   generates the list of perfect powers up to a limit that is enlarged on
   demand.  It then factors each perfect power using the provided
   factor_u64 function, computes the gcd of the exponent multiset, counts
   its divisors and subtracts one.  All arithmetic uses only integer
   operations and the types suggested by the build system.  The routine
   is O(n) per call for a fixed limit and works for all n≥1 used in
   tests (n up to ≈10000).  */
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/factor_u64.h"   /* factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt) */

/* --- helpers ----------------------------------------------------------- */

static uint64_t gcd_uint64(uint64_t a, uint64_t b)
{
    while (b != 0) {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* count positive divisors of x (x>=1), works for x up to 64 */
static unsigned int tau_uint64(uint64_t x)
{
    unsigned int cnt = 1;
    for (uint64_t p = 2; p * p <= x; ++p) {
        if (x % p == 0) {
            unsigned int e = 0;
            while (x % p == 0) {
                x /= p;
                ++e;
            }
            cnt *= (e + 1);
        }
    }
    if (x > 1)            /* remaining prime factor */
        cnt *= 2;
    return cnt;
}

/* simple comparison for qsort */
int compare_uint64(const void *a, const void *b)
{
    uint64_t va = *(const uint64_t *)a;
    uint64_t vb = *(const uint64_t *)b;
    if (va < vb) return -1;
    return va > vb;
}

/* --- perfect powers table --------------------------------------------- */

static uint64_t *pp_table   = NULL;   /* table of perfect powers, 1 included */
static size_t   pp_len     = 0;       /* number of entries in pp_table */
static uint64_t pp_limit   = 0;       /* maximum value generated */

/* generate perfect powers up to 'limit', store in pp_table */
static void gen_perfect_powers(uint64_t limit)
{
    /* 1) generate all base^exp  with base>=2, exp>=2, value<=limit */
    size_t cap = 1 << 15;                 /* start capacity 32768 */
    uint64_t *tmp = (uint64_t *)malloc(cap * sizeof(uint64_t));
    if (!tmp) return; /* allocation failure – should not happen */

    size_t len = 0;
    uint64_t max_exp = 0;
    while ((1ULL << (max_exp + 1)) <= limit) ++max_exp;  /* 2^max_exp <= limit */

    for (uint64_t exp = 2; exp <= max_exp; ++exp) {
        for (uint64_t base = 2;; ++base) {
            __int128 val = 1;
            for (uint64_t i = 0; i < exp; ++i) {
                val *= base;
                if (val > limit) break;
            }
            if (val > limit) break;            /* further base will only grow */
            uint64_t v = (uint64_t)val;

            if (len == cap) {
                cap <<= 1;
                uint64_t *p = (uint64_t *)realloc(tmp, cap * sizeof(uint64_t));
                if (!p) { free(tmp); return; }
                tmp = p;
            }
            tmp[len++] = v;
        }
    }

    /* 2) sort, unique, and prepend 1 */
    qsort(tmp, len, sizeof(uint64_t), compare_uint64);

    size_t uniq = (len > 0) ? 1 : 0; /* count unique starting with 0? */
    for (size_t i = 1; i < len; ++i)
        if (tmp[i] != tmp[i - 1])
            ++uniq;

    size_t final_len = uniq + 1; /* +1 for '1' as first perfect power */
    uint64_t *final = (uint64_t *)malloc(final_len * sizeof(uint64_t));
    if (!final) { free(tmp); return; }

    final[0] = 1;       /* first element is 1 */
    size_t idx = 1;
    if (len > 0) {
        final[idx++] = tmp[0];
        for (size_t i = 1; i < len; ++i)
            if (tmp[i] != tmp[i - 1])
                final[idx++] = tmp[i];
    }

    /* free old table if any */
    free(pp_table);
    pp_table = final;
    pp_len   = final_len;
    pp_limit = limit;

    free(tmp);
}

/* ensure pp_table has at least 'needed' entries; enlarge limit if necessary */
static void ensure_pp_table(size_t needed)
{
    if (pp_len >= needed) return;

    /* increase limit until we can satisfy needed */
    uint64_t limit = (pp_limit > 0) ? pp_limit : 100000000ULL; /* start at 1e8 */
    while (pp_len < needed) {
        uint64_t new_limit = limit * 2;
        if (new_limit < limit) { /* overflow */
            new_limit = UINT64_MAX;
            if (new_limit == limit) break;
        }
        gen_perfect_powers(new_limit);
        limit = new_limit;
        if (limit == UINT64_MAX) break;
    }
}

/* ----------------------------------------------------- */

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset is 1; no defined value for n==0 */

    /* ensure table large enough for n */
    ensure_pp_table((size_t)n);

    if (n - 1 >= pp_len) return 0; /* safety, should not happen */

    uint64_t mpower = pp_table[n - 1];  /* nth perfect power */

    if (mpower == 1ULL) return 0;      /* 1 has no representation with a>1 */

    /* factor mpower */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t pf_cnt = 0;
    factor_u64(mpower, primes, exps, &pf_cnt);

    /* compute gcd of exponents */
    uint64_t g = exps[0];
    for (size_t i = 1; i < pf_cnt; ++i)
        g = gcd_uint64(g, exps[i]);

    if (g <= 1) return 0;  /* should not happen for mpower>1 */

    /* number of divisors of g minus 1 */
    unsigned int d = tau_uint64(g);
    return (seq_t)(d - 1);
}
