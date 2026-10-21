/*
 * A273938  Sum of the divisors of the n-th odd prime power.
 * https://oeis.org/A273938
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The OEIS sequence A273938: Sum of the divisors of the n‑th odd prime power.  
   a(n) = σ(p_k^e_k) where the odd prime powers p_k^e_k are listed in
   increasing order, with the first element being 1.  The function
   returns the nth term (a(1)=1).  The implementation pre‑computes a
   sorted list of all odd prime powers up to a fixed bound that
   guarantees at least 10 000 terms.  It is valid C11 and C++17. */

#include <stdlib.h>
#include <stdbool.h>

/* Utility: build a list of primes up to a given limit */
#include "../utils/build_primes_upto.h"

typedef struct {
    idx_t val;   /* odd prime power (fits in idx_t) */
    seq_t sigma; /* σ(odd prime power) */
} PrimePower;

/* Global cache ---------------------------------------------------------- */
static bool built = false;
static PrimePower *cache = NULL;
static size_t cache_size = 0;

/* Helper: compare by prime power value (used by qsort in C) */
#if !defined(__cplusplus)
static int compare_pp(const void *a, const void *b)
{
    const PrimePower *x = (const PrimePower *)a;
    const PrimePower *y = (const PrimePower *)b;
    if (x->val < y->val) return -1;
    if (x->val > y->val) return 1;
    return 0;
}
#endif

/* Pre‑compute the sorted list of odd prime powers and their σ values */
static void ensure_precomputed(void)
{
    if (built) return;      /* already done */

    /* Choose a bound large enough for n≤10000 */
    const idx_t LIMIT = 200000;   /* 2×10^5 is safe */

    /* Get all primes up to LIMIT (including 2) */
    uint64_t *pr_list = NULL;
    size_t prime_cnt = 0;
    build_primes_upto(LIMIT, &pr_list, &prime_cnt);

    /* Temporary array for collecting prime powers */
#if defined(__cplusplus)
    std::vector<PrimePower> tmp;
    tmp.reserve(20000);           /* generous initial capacity */
#else
    size_t tmpsz = 0;
    size_t tmpcap = 20000;
    PrimePower *tmp = (PrimePower *)malloc(tmpcap * sizeof(PrimePower));
    if (!tmp) { free(pr_list); return; }
#endif

    /* Include 1 as the first odd prime power (σ(1)=1) */
#if defined(__cplusplus)
    tmp.push_back({1, (seq_t)1});
#else
    tmp[tmpsz].val = 1;
    tmp[tmpsz].sigma = (seq_t)1;
    ++tmpsz;
#endif

    /* Process each odd prime and generate its powers */
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = pr_list[i];
        if (p == 2) continue;            /* skip 2 */

        seq_t pow = 1;          /* p^0 initially */
        seq_t sigma_sum = 1;    /* σ(p^0) = 1 */
        while (true) {
            pow = pow * (seq_t)p;
            if (pow > (seq_t)LIMIT) break;   /* exceeds bound */

            sigma_sum += pow;                /* σ(p^e) = σ(p^(e-1)) + p^e */

#if defined(__cplusplus)
            tmp.push_back({(idx_t)pow, sigma_sum});
#else
            if (tmpsz >= tmpcap) {
                tmpcap *= 2;
                PrimePower *tmp2 = (PrimePower *)realloc(tmp, tmpcap * sizeof(PrimePower));
                if (!tmp2) { free(tmp); free(pr_list); return; }
                tmp = tmp2;
            }
            tmp[tmpsz].val = (idx_t)pow;
            tmp[tmpsz].sigma = sigma_sum;
            ++tmpsz;
#endif
        }
    }
    free(pr_list);

    /* Sort by prime power value */
#if defined(__cplusplus)
    std::sort(tmp.begin(), tmp.end(),
              [](const PrimePower &a, const PrimePower &b) { return a.val < b.val; });
    cache_size = tmp.size();
    cache = (PrimePower *)malloc(cache_size * sizeof(PrimePower));
    if (!cache) { tmp.clear(); return; }
    for (size_t i = 0; i < cache_size; ++i) cache[i] = tmp[i];
    tmp.clear();
#else
    qsort(tmp, tmpsz, sizeof(PrimePower), compare_pp);
    cache_size = tmpsz;
    cache = tmp;      /* temp array becomes cache */
#endif

    built = true;
}

/* Public sequence function ------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 1) return 0;             /* undefined for 0 by OEIS convention */

    ensure_precomputed();
    if ((idx_t)n > cache_size) return 0;   /* safety guard */

    return cache[n - 1].sigma;
}
