/*
 * A308404  a(n) = Q(A001359(n)), with Q(n) = (2^(n+2)-3n-8)/(n*(n+2)).
 * https://oeis.org/A308404
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Q(A001359(n)) * A001359(n) = lower prime of the n-th
 *            twin‑prime pair * Q(m) = (2^(m+2) - 3m - 8) / (m*(m+2)) *
 *            Offset: 1 (a(1) is the first term shown in OEIS) * The function
 *            is memoised; it works for any n≥1 and any build * (uint64_t,
 *            uint128_t or arbitrary‑precision) that supplies seq_t
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 8, u128 n <= 10,
 *            big any n
 * Helpers:   big_pow, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"   /* is_prime_big(seq_t) */
#include "../utils/big_pow.h"       /* big_pow(seq_t base, seq_t exp) */

static seq_t *a_cache = NULL;     /* cache of a(n), 1‑based, index 0 unused */
static size_t   a_cache_cap = 0;   /* allocated size of a_cache          */
static idx_t    a_cache_max = 0;   /* largest n that has been cached     */

static uint64_t *twin_list = NULL; /* array of lower primes in twin pairs */
static size_t   twin_cap  = 0;     /* allocated capacity of twin_list    */
static idx_t    twin_cnt  = 0;     /* number of twin primes stored      */

/* ------------------------------------------------------------------ */
/* Ensure twin_list contains at least n twin‑prime lower members.     */
/* ------------------------------------------------------------------ */
static void
ensure_twin_primes(idx_t n)
{
    if (twin_cnt >= n)
        return;

    if (twin_cap == 0) {
        twin_cap   = 64;
        twin_list  = (uint64_t *)calloc(twin_cap, sizeof(uint64_t));
    }

    uint64_t cand = (twin_cnt == 0) ? 3 : twin_list[twin_cnt - 1] + 2;

    while (twin_cnt < n) {
        if (cand > UINT64_MAX - 2)
            break;  /* safety – should never happen for n ≤ 10000 */

        if (is_prime_big((seq_t)cand) && is_prime_big((seq_t)(cand + 2))) {
            if (twin_cnt >= twin_cap) {
                twin_cap *= 2;
                twin_list = (uint64_t *)realloc(twin_list, twin_cap * sizeof(uint64_t));
            }
            twin_list[twin_cnt++] = cand;
        }
        cand += 2;  /* jump to next odd candidate */
    }
}

/* ------------------------------------------------------------------ */
/* Ensure a_cache can hold indices up to n (1‑based).                 */
/* ------------------------------------------------------------------ */
static void
ensure_cache(idx_t n)
{
    if (a_cache_max >= n)
        return;

    size_t new_cap = n + 1;  /* index 0 unused */

    if (new_cap > a_cache_cap) {
        if (a_cache == NULL) {
            a_cache = (seq_t *)calloc(new_cap, sizeof(seq_t));
        } else {
            a_cache = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
        }
        a_cache_cap = new_cap;
    }
}

/* ------------------------------------------------------------------ */
/* Main sequence function.                                            */
/* ------------------------------------------------------------------ */
seq_t
a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* undefined for n < 1, safe return */

    ensure_twin_primes(n);
    ensure_cache(n);

    /* Compute missing terms in ascending order. */
    for (idx_t i = a_cache_max + 1; i <= n; ++i) {
        uint64_t p = twin_list[i - 1];          /* lower twin prime */

        /* The exponent in the power of two. */
        idx_t exp = (idx_t)p + 2;

        seq_t q;   /* the value to cache */

        if (exp <= 127) {
            /* Use 128‑bit arithmetic for the intermediate values. */
            unsigned __int128 pow128  = ((unsigned __int128)1) << exp;
            unsigned __int128 num128  = pow128
                                        - (unsigned __int128)(3 * (unsigned __int128)p)
                                        - 8;
            unsigned __int128 denom128 = (unsigned __int128)p * (p + 2);
            unsigned __int128 quo128 = num128 / denom128;
            q = (seq_t)quo128;  /* cast back to seq_t */
        } else {
            /* For very large p we fall back to arbitrary‑precision power. */
            seq_t pow = big_pow((seq_t)2, (seq_t)exp);
            seq_t num = pow;
            num -= (seq_t)(3 * p);
            num -= (seq_t)8;
            seq_t denom = (seq_t)p * (seq_t)(p + 2);
            q = num / denom;  /* exact division */
        }

        a_cache[i] = q;
    }

    a_cache_max = n;
    return a_cache[n];
}
