/*
 * A367685  Numbers divisible by their multiset multiplicity kernel.
 * https://oeis.org/A367685
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   A367685 – numbers divisible by their multiset multiplicity
 *            kernel
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation performed by enumerating integers, computing the MMK, and selecting those divisible by it. */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* ---------- global static data ---------- */
static seq_t *a_cache      = NULL;  /* cached sequence values */
static size_t a_cache_cap  = 0;
static size_t a_cache_len  = 0;
static uint64_t next_to_check = 1;   /* next integer to test */

static uint64_t *spf       = NULL;  /* smallest prime factor table */
static size_t    spf_limit = 0;

/* ---------- helper: build smallest prime factor table ---------- */
static void build_spf(size_t limit, uint64_t **out)
{
    uint64_t *arr = (uint64_t *)malloc((limit + 1) * sizeof(uint64_t));
    if (!arr) abort();
    for (size_t i = 0; i <= limit; ++i) arr[i] = 0;

    for (size_t i = 2; i <= limit; ++i) {
        if (arr[i] == 0) {
            arr[i] = (uint64_t)i;
            if (i <= limit / i) {
                for (size_t j = i * i; j <= limit; j += i)
                    if (arr[j] == 0) arr[j] = (uint64_t)i;
            }
        }
    }
    *out = arr;
}

/* ---------- ensure that SPF table covers at least n ---------- */
static void ensure_spf_limit(uint64_t n)
{
    if (n <= spf_limit) return;
    uint64_t *new_spf;
    build_spf((size_t)n, &new_spf);
    free(spf);
    spf       = new_spf;
    spf_limit = (size_t)n;
}

/* ---------- compute multiset multiplicity kernel of v ---------- */
static seq_t compute_mmk(uint64_t v)
{
    if (v <= 1) return 1;

    uint64_t n = v;
    int max_exp = 0;
    int min_prime[32] = {0};
    int count_exp[32] = {0};

    while (n > 1) {
        uint64_t p = spf[n];
        if (p == 0) p = n;
        int e = 0;
        while (n % p == 0) {
            n /= p;
            ++e;
        }
        if (e > max_exp) max_exp = e;
        if (min_prime[e] == 0) min_prime[e] = (int)p;
        ++count_exp[e];
    }

    seq_t mmk = 1;
    for (int e = 1; e <= max_exp; ++e) {
        if (min_prime[e] != 0) {
            seq_t term = 1;
            for (int i = 0; i < count_exp[e]; ++i) term *= (seq_t)min_prime[e];
            mmk *= term;
        }
    }
    return mmk;
}

/* ---------- ensure cache up to index n ---------- */
static void ensure_a_cache(idx_t n)
{
    while (a_cache_len < n) {
        /* Ensure SPF table is large enough */
        if (next_to_check > spf_limit)
            ensure_spf_limit((uint64_t)next_to_check * 2 + 100);

        uint64_t v = next_to_check;
        seq_t mmk  = compute_mmk(v);
        seq_t val  = (seq_t)v;

        if (mmk != (seq_t)0 && val % mmk == 0) {
            if (a_cache_len == a_cache_cap) {
                size_t new_cap = a_cache_cap ? a_cache_cap * 2 : 64;
                seq_t *tmp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();
                a_cache = tmp;
                a_cache_cap = new_cap;
            }
            a_cache[a_cache_len++] = val;
        }
        ++next_to_check;
    }
}

/* ---------- public interface ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n==0, but avoid crash */
    ensure_a_cache(n);
    return a_cache[n - 1];
}
