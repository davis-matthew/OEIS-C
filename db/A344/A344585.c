/*
 * A344585  Numbers whose sum of proper divisors (A001065) is greater than
 *          their arithmetic derivative (A003415).
 * https://oeis.org/A344585
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) computed by searching integers in increasing order and
 *            selecting those for which the sum of proper divisors exceeds the
 *            arithmetic derivative
 * Range:     u8 n <= 38, u16 n <= 8754, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 88 terms through n = 9384 (2026-11-03)
 * Helpers:   factor_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/mem.h"

#ifdef __cplusplus
#define OEISC_VPP(p) (p)
#define OEISC_VP(p) (p)
#else
#define OEISC_VPP(p) ((void **)(p))
#define OEISC_VP(p) ((void *)(p))
#endif

/* Grow a dynamic array when needed.  The pointer is passed as OEISC_VPP(.) */
#ifdef __cplusplus
template <class T>
static void ensure_capacity(size_t needed, size_t *cap, T **ptr, size_t elem_size)
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
    while (new_cap < needed) new_cap <<= 1;
    T *tmp = mem_resize(T, *ptr, (new_cap * elem_size) / sizeof(T));
    if (!tmp) abort();               /* on OOM we abort as required */
    *ptr = tmp;
    *cap = new_cap;
}
#else
static void ensure_capacity(size_t needed, size_t *cap, void **ptr, size_t elem_size)
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
    while (new_cap < needed) new_cap <<= 1;
    void *tmp = realloc(*ptr, new_cap * elem_size);
    if (!tmp) abort();               /* on OOM we abort as required */
    *ptr = tmp;
    *cap = new_cap;
}
#endif

/* Helper: test if a given integer m satisfies the condition. */
static bool check_uint64(uint64_t m) {
    /* factorization arrays – enough for any 64‑bit integer */
    uint64_t pr[64], ex[64];
    size_t cnt;

    if (m < 2) return false;
    factor_u64(m, pr, ex, &cnt);

    seq_t m_t = (seq_t)m;
    seq_t sigma = 1;

    /* compute σ(m) = product over prime powers of (p^(e+1)-1)/(p-1) */
    for (size_t i = 0; i < cnt; ++i) {
        seq_t sum_powers = 1; /* includes p^0 */
        seq_t pow     = 1;
        for (size_t k = 0; k < ex[i]; ++k) {
            pow *= (seq_t)pr[i];
            sum_powers += pow;
        }
        sigma *= sum_powers;
    }

    seq_t sum_proper = sigma - m_t;

    /* compute arithmetic derivative: n' = Σ e_i * n / p_i */
    seq_t deriv = 0;
    for (size_t i = 0; i < cnt; ++i) {
        deriv += (m_t / (seq_t)pr[i]) * (seq_t)ex[i];
    }

    return sum_proper > deriv;
}

/* static cache of found terms */
static seq_t  *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;
static uint64_t last_checked = 1;

/* The required function. */
seq_t a(idx_t n) {
    if (n == 0) return 0;          /* offset is 1 – undefined for n=0 */

    /* extend cache until we have at least n terms */
    while (cache_len < (size_t)n) {
        ++last_checked;
        if (check_uint64(last_checked)) {
            ensure_capacity(cache_len + 1, &cache_cap, OEISC_VPP(&cache), sizeof(seq_t));
            cache[cache_len++] = (seq_t)last_checked;
        }
    }
    return cache[n - 1];
}
