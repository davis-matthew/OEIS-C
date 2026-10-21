/*
 * A274876  The number of ways (2n)^2 is expressible as (p+1)(q+1) where p and
 *          q are distinct primes.
 * https://oeis.org/A274876
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of unordered factorizations of (2n)^2 into
 *            (p+1)(q+1) with p and q distinct primes
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* ---------- Prime infrastructure ------------------------------------ */
static void init_primes(void);
static inline bool isPrime(uint64_t x);
static void factor_uint32(uint32_t n, uint32_t *out_pr, uint32_t *out_exp, size_t *cnt);

static uint32_t *prime_list      = NULL;
static size_t   prime_count      = 0;
static uint32_t *spf              = NULL;
static size_t   spf_limit         = 0;
static bool     primes_inited    = false;

static void init_primes(void)
{
    if (primes_inited) return;

    /* limit large enough for n up to ~10000 (sqrt(4*n^2) <= 20000) */
    spf_limit  = 20000;
    bool *is_comp = (bool *)calloc(spf_limit + 1, sizeof(bool));
    if (!is_comp) exit(1);

    for (size_t i = 2; i * i <= spf_limit; ++i)
        if (!is_comp[i])
            for (size_t j = i * i; j <= spf_limit; j += i)
                is_comp[j] = true;

    for (size_t i = 2; i <= spf_limit; ++i)
        if (!is_comp[i]) ++prime_count;

    prime_list = (uint32_t *)malloc(prime_count * sizeof(uint32_t));
    size_t pi = 0;
    for (size_t i = 2; i <= spf_limit; ++i)
        if (!is_comp[i]) prime_list[pi++] = (uint32_t)i;
    free(is_comp);

    spf = (uint32_t *)calloc(spf_limit + 1, sizeof(uint32_t));
    for (size_t i = 2; i <= spf_limit; ++i) {
        if (spf[i] == 0) {
            spf[i] = (uint32_t)i;
            if (i * i <= spf_limit)
                for (size_t j = i * i; j <= spf_limit; j += i)
                    if (spf[j] == 0) spf[j] = (uint32_t)i;
        }
    }
    primes_inited = true;
}

static inline bool isPrime(uint64_t x)
{
    if (x < 2) return false;
    for (size_t i = 0; i < prime_count &&
                         (uint64_t)prime_list[i] * prime_list[i] <= x; ++i)
        if (x % prime_list[i] == 0) return false;
    return true;
}

static void factor_uint32(uint32_t n, uint32_t *out_pr, uint32_t *out_exp, size_t *cnt)
{
    *cnt = 0;
    while (n > 1) {
        uint32_t p = spf[n];
        uint32_t e = 0;
        while (n % p == 0) { n /= p; ++e; }
        out_pr[(*cnt)] = p;
        out_exp[(*cnt)] = e;
        ++(*cnt);
    }
}

/* ---------- Enumerate divisors of 4*n^2 ------------------------------ */
static void dfs_div(size_t pos, uint64_t cur,
                    const uint32_t *fpr, const uint32_t *fexp,
                    size_t fcnt, uint64_t total, seq_t *cnt)
{
    if (pos == fcnt) {
        uint64_t d = cur;
        uint64_t g = total / d;
        if (d > g) return;          /* unordered pairs only */
        if (d == g) return;          /* would give p == q, not distinct */
        uint64_t p = d - 1;
        uint64_t q = g - 1;
        if (isPrime(p) && isPrime(q)) ++(*cnt);
        return;
    }

    uint64_t mult = 1;
    for (uint32_t e = 0; e <= fexp[pos]; ++e) {
        dfs_div(pos + 1, cur * mult, fpr, fexp, fcnt, total, cnt);
        mult *= fpr[pos];
    }
}

/* ---------- Sequence function ------------------------------------- */
seq_t a(idx_t n)
{
    if (!primes_inited) init_primes();

    if (n < 1) return 0; /* sequence starts at offset 1 */

    /* factor n */
    uint32_t npr[16], nexp[16];
    size_t ncnt;
    factor_uint32((uint32_t)n, npr, nexp, &ncnt);

    /* factorization of 4*n^2 : 4 = 2^2, each prime exponent doubled */
    uint32_t fpr[16];
    uint32_t fexp[16];
    size_t fcnt = 0;

    /* exponent of 2 in n */
    uint32_t exp2 = 0;
    for (size_t i = 0; i < ncnt; ++i)
        if (npr[i] == 2) { exp2 = nexp[i]; break; }

    fpr[fcnt] = 2;
    fexp[fcnt++] = 2 + 2 * exp2;   /* 2 from 4 + doubled exponent from n */

    for (size_t i = 0; i < ncnt; ++i) {
        if (npr[i] == 2) continue;
        fpr[fcnt] = npr[i];
        fexp[fcnt++] = 2 * nexp[i];
    }

    uint64_t total = 4ULL * (uint64_t)n * (uint64_t)n;
    seq_t cnt = 0;
    dfs_div(0, 1ULL, fpr, fexp, fcnt, total, &cnt);
    return cnt;
}
