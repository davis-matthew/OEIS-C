/*
 * A280348  Irregular triangle read by rows in which row n lists the divisors
 *          of the sum of the divisors of n.
 * https://oeis.org/A280348
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 980, u16 n <= 8794, u32 n <= 8794, u64 n <= 8794,
 *            u128 n <= 8794, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A280348 – irregular triangle read by rows.
    Each row n lists the sorted divisors of σ(n) (sum of divisors of n).
    The sequence a(n) is obtained by reading these rows left‑to‑right.
    The routine caches results between calls and works for any seq_t
    (1, 2, 4, 8, 16, 32, 64 or 128‑bit unsigned, or arbitrary precision).
------------------------------------------------------------------- */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/factor_u64.h"

static uint64_t *sigma_arr = NULL;   /* σ(n) for each n */
static idx_t *cum_arr   = NULL;     /* cumulative divisor counts up to row */
static idx_t max_computed = 0;      /* highest n that has been processed */

/* ------------------------------------------------------------------ */
/* Ensure internal arrays are computed up to at least index n */
static void ensure_up_to(idx_t n)
{
    if (n <= max_computed) return;

    size_t new_cap = (size_t)n + 10;
    if (sigma_arr == NULL) {
        sigma_arr = (uint64_t *)calloc(new_cap, sizeof(uint64_t));
        cum_arr   = (idx_t *)calloc(new_cap,  sizeof(idx_t));
    } else {
        sigma_arr = (uint64_t *)realloc(sigma_arr, new_cap * sizeof(uint64_t));
        cum_arr   = (idx_t *)realloc(cum_arr,   new_cap * sizeof(idx_t));
    }
    if (!sigma_arr || !cum_arr) abort();

    uint64_t pr[64];
    uint64_t ex[64];
    size_t   cnt;

    for (idx_t i = max_computed + 1; i <= n; ++i) {
        uint64_t sigma_i;
        if (i == 1) {
            sigma_i = 1;
        } else {
            factor_u64((uint64_t)i, pr, ex, &cnt);
            sigma_i = 1;
            for (size_t j = 0; j < cnt; ++j) {
                uint64_t p   = pr[j];
                uint64_t exp = ex[j];
                uint64_t pow_val = 1;
                for (uint64_t t = 0; t <= exp; ++t) pow_val *= p;
                uint64_t term = (pow_val - 1) / (p - 1);
                sigma_i *= term;
            }
        }
        sigma_arr[i] = sigma_i;

        factor_u64(sigma_i, pr, ex, &cnt);
        uint64_t dcnt = 1;
        for (size_t j = 0; j < cnt; ++j) dcnt *= (ex[j] + 1);

        cum_arr[i] = cum_arr[i - 1] + (idx_t)dcnt;
    }
    max_computed = n;
}

/* ------------------------------------------------------------------ */
/* Helper for qsort of uint64_t */
static int cmp_uint64(const void *a, const void *b)
{
    uint64_t va = *(const uint64_t *)a;
    uint64_t vb = *(const uint64_t *)b;
    return (va < vb) ? -1 : (va > vb) ? 1 : 0;
}

/* ------------------------------------------------------------------ */
/* Recursive generation of all divisors of a number from its primes */
static void gen_divisors(const uint64_t *pf, const uint64_t *pe,
                         size_t pf_cnt, size_t idx,
                         uint64_t cur, uint64_t *out, size_t *pos)
{
    if (idx == pf_cnt) {
        out[(*pos)++] = cur;
        return;
    }
    uint64_t val = 1;
    for (uint64_t e = 0; e <= pe[idx]; ++e) {
        gen_divisors(pf, pe, pf_cnt, idx + 1, cur * val, out, pos);
        val *= pf[idx];
    }
}

/* ------------------------------------------------------------------ */
/* Return the k‑th smallest divisor of num (1‑based). */
static uint64_t kth_divisor(uint64_t num, idx_t k)
{
    uint64_t pr[64];
    uint64_t ex[64];
    size_t   cnt;

    factor_u64(num, pr, ex, &cnt);

    size_t total = 1;
    for (size_t i = 0; i < cnt; ++i) total *= (ex[i] + 1);

    uint64_t *divs = (uint64_t *)malloc(total * sizeof(uint64_t));
    if (!divs) abort();

    size_t pos = 0;
    gen_divisors(pr, ex, cnt, 0, 1, divs, &pos);

    qsort(divs, total, sizeof(uint64_t), cmp_uint64);

    uint64_t ans = divs[k - 1];
    free(divs);
    return ans;
}

/* ------------------------------------------------------------------ */
/* Public sequence interface. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* safety for offset 1 */

    ensure_up_to(n);

    /* Locate the row that contains the nth term. */
    idx_t lo = 1, hi = max_computed;
    while (lo < hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        if (cum_arr[mid] >= n) hi = mid;
        else                   lo = mid + 1;
    }
    idx_t row = lo;
    idx_t k   = n - cum_arr[row - 1];   /* 1‑based index within this row */

    uint64_t sigma_val = sigma_arr[row];
    uint64_t divisor   = kth_divisor(sigma_val, k);

    return (seq_t)divisor;
}
