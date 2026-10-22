/*
 * A343940  Sum of numbers of ways to choose a k-chain of divisors of n - k,
 *          for k = 0..n - 1.
 * https://oeis.org/A343940
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} tau_k(n+1-k) where tau_k(m) =
 *            product_{p^e||m} C(e+k-1, k-1) (ordered factorizations)
 * Range:     u8 n <= 12, u16 n <= 38, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   build_spf, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/gcd.h"
#include "../utils/build_spf.h"   /* build_spf(limit, &ptr) */

#define MAX_PRIMES_PER_N 6

typedef struct {
    int cnt;                     /* number of distinct primes */
    int primes[MAX_PRIMES_PER_N];
    int exps[MAX_PRIMES_PER_N];
} factor_t;

/* static caches -------------------------------------------------------- */

static idx_t *spf = NULL;           /* smallest prime factor array */
static idx_t spf_limit = 0;         /* current size of spf array */

static factor_t *factor_cache = NULL;
static idx_t cache_limit = 0;       /* current size of factor_cache */

/* helper: ensure spf array up to n ------------------------------------- */
static void ensure_spf(idx_t n)
{
    if (n <= spf_limit) return;
    if (spf) free(spf);
    build_spf(n, &spf);
    spf_limit = n;
}

/* helper: factorize numbers up to n, caching results ------------------ */
static void ensure_factors(idx_t n)
{
    if (n <= cache_limit) return;
    factor_t *new_cache = (factor_t *)realloc(factor_cache,
                                              sizeof(factor_t) * (n + 1));
    if (!new_cache) exit(EXIT_FAILURE);   /* allocation failure */
    factor_cache = new_cache;
    for (idx_t m = cache_limit + 1; m <= n; ++m) {
        idx_t temp = m;
        int cnt = 0;
        while (temp > 1) {
            idx_t p = spf[temp];
            int e = 0;
            while (temp % p == 0) { temp /= p; ++e; }
            factor_cache[m].primes[cnt] = (int)p;
            factor_cache[m].exps[cnt]   = e;
            ++cnt;
        }
        factor_cache[m].cnt = cnt;
    }
    cache_limit = n;
}

/* helper: small binomial C(e + k - 1, k - 1) with e <= 13 ---------------- */
static seq_t comb_small(int e, idx_t k_minus1)
{
    if (e == 0) return (seq_t)1;
    seq_t res = (seq_t)1;
    for (int i = 1; i <= e; ++i) {
        seq_t mult = (seq_t)k_minus1 + (seq_t)i;
        seq_t div  = (seq_t)i;
        seq_t g = gcd(mult, div);
        mult /= g;   div /= g;
        seq_t g2 = gcd(res, div);
        res /= g2;   div /= g2;   /* div should now be 1 */
        res = res * mult;
    }
    return res;
}

/* main function ------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;    /* undefined, but safe */

    ensure_spf(n);
    ensure_factors(n);

    seq_t sum = (seq_t)0;
    for (idx_t m = (idx_t)1; m <= n; ++m) {
        idx_t k_minus1 = n - m;            /* k = n+1-m  => k-1 = n-m */
        seq_t val = (seq_t)1;
        factor_t *f = &factor_cache[m];
        for (int i = 0; i < f->cnt; ++i) {
            int e = f->exps[i];
            val = val * comb_small(e, k_minus1);
        }
        sum += val;
    }
    return sum;
}
