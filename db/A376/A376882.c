/*
 * A376882  a(n) is the product of the Zumkeller divisors of n.
 * https://oeis.org/A376882
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product of the Zumkeller divisors of n
 * Range:     u8 n <= 23, u16 n <= 47, u32 n <= 119, u64 n <= 419,
 *            u128 n <= 1679, big any n
 * Helpers:   build_spf, compute_divisors, factor_u64, is_zumkeller
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "../utils/factor_u64.h"   /* factor_u64 */
#include "../utils/build_spf.h"    /* build_spf */
#include "../utils/compute_divisors.h"
#include "../utils/is_zumkeller.h"

/* Static cache for smallest prime factor array up to current limit. */
static idx_t *spf_cache = NULL;          /* smallest prime factor */
static idx_t  spf_cap = 0;               /* allocated size */

/* Static cache for Zumkeller flag up to current limit (1-based). */
static bool   *zum_cache = NULL;         /* true iff number is Zumkeller */
static idx_t   zum_cap   = 0;             /* allocated size */

/* Ensure the spf array has room for numbers up to n (inclusive). */
static void ensure_spf(idx_t n)
{
    if (n <= spf_cap) return;
    if (spf_cache) free(spf_cache);
    build_spf(n, &spf_cache);
    spf_cap = n;
}

/* Ensure the Zumkeller cache up to n (inclusive). */
static void ensure_zum(idx_t n, idx_t *spf)
{
    if (n <= zum_cap) return;
    bool *new_cache = (bool*)malloc((n + 1) * sizeof(bool));
    if (!new_cache) return;            /* allocation failure, ignore */
    if (zum_cache) {
        memcpy(new_cache, zum_cache, (zum_cap + 1) * sizeof(bool));
        free(zum_cache);
    }
    for (idx_t i = zum_cap + 1; i <= n; ++i) {
        new_cache[i] = is_zumkeller((uint64_t)i, spf);
    }
    zum_cache = new_cache;
    zum_cap   = n;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 1) return 1;                /* undefined for n<1, return 1 */

    /* Precompute smallest prime factors up to n. */
    ensure_spf(n);
    /* Precompute Zumkeller flags up to n. */
    ensure_zum(n, spf_cache);

    /* Compute product of Zumkeller divisors of n. */
    uint64_t pr[32];
    uint64_t ex[32];
    size_t cnt = 0;
    uint64_t temp = (uint64_t)n;
    while (temp > 1 && cnt < 32) {
        uint64_t p = spf_cache[temp];
        if (p == 0) p = temp;          /* temp is prime */
        uint64_t e = 0;
        while (temp % p == 0) {
            temp /= p;
            ++e;
        }
        pr[cnt] = p;
        ex[cnt] = e;
        ++cnt;
    }
    uint64_t divs[256];
    size_t div_cnt;
    compute_divisors(n, pr, ex, cnt, divs, &div_cnt);

    seq_t prod = 1;
    for (size_t i = 0; i < div_cnt; ++i) {
        uint64_t d = divs[i];
        if (d == 1) continue;          /* 1 is not Zumkeller */
        if (zum_cache[d]) {
            prod *= (seq_t)d;
        }
    }
    return prod;
}
