/*
 * A389215  Numbers k such that {sum of unitary divisors of k} plus {sum of
 *          squarefree divisors of k} >= 2*k.
 * https://oeis.org/A389215
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   A389215: a(n) is the nth positive integer k such that sigma_u(k)
 *            + sigma_sf(k) ≥ 2*k, where sigma_u is the sum of unitary
 *            divisors and sigma_sf is the sum of squarefree divisors
 * Range:     u8 n <= 49, u16 n <= 9999, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 96 terms through n = 9055 (2026-11-03)
 * Helpers:   build_spf, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/build_spf.h"
#include "../utils/mem.h"

/* Helper to grow a sequence array of type seq_t. */
static void grow_seq_array(seq_t **arr, size_t *size, size_t new_len)
{
    if (*size >= new_len) return;
    size_t new_cap = (*size > 0) ? (*size << 1) : 16;
    while (new_cap < new_len) new_cap <<= 1;
    seq_t *tmp = mem_resize(seq_t, *arr, new_cap);
    if (!tmp) abort();
    *arr = tmp;
    *size = new_cap;
}

/* Smallest prime factor table */
static idx_t *spf = NULL;
static idx_t spf_limit = 0;

/* Cache of qualifying numbers */
static seq_t *cache      = NULL;
static size_t cache_len  = 0;
static size_t cache_cap  = 0;

/* Next k to be tested */
static idx_t next_k = 1;

/* Ensure spf[] covers numbers up to limit. */
static void ensure_spf(idx_t limit)
{
    if (limit <= spf_limit)
        return;
    if (spf)
        free(spf);
    build_spf(limit, &spf);
    spf_limit = limit;
}

/* Append a new qualifying number to the cache. */
static void cache_append(seq_t val)
{
    if (cache_len == cache_cap) {
        size_t need = cache_len + 1;          /* at least one more slot */
        grow_seq_array(&cache, &cache_cap, need);
    }
    cache[cache_len++] = val;
}

/* Test whether k satisfies the inequality. */
static bool qualifies(idx_t k)
{
    if (k == 0)
        return false;
    ensure_spf(k);

    idx_t tmp = k;
    seq_t U = 1;  /* sum of unitary divisors */
    seq_t S = 1;  /* sum of squarefree divisors */

    while (tmp > 1) {
        idx_t p = spf[tmp];
        idx_t e = 0;
        while (tmp % p == 0) {
            tmp /= p;
            ++e;
        }
        /* p^e */
        seq_t ppow = 1;
        for (idx_t i = 0; i < e; ++i)
            ppow *= (seq_t)p;
        U *= (ppow + 1);
        S *= ((seq_t)p + 1);
    }

    seq_t sum   = U + S;
    seq_t two_k = (seq_t)k * 2;
    return sum >= two_k;
}

/* Public sequence function: returns the nth qualifying integer. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;        /* offset is 1; undefined value for n=0 */

    while (cache_len < n) {
        if (qualifies(next_k))
            cache_append((seq_t)next_k);
        ++next_k;
    }
    return cache[n - 1];
}
