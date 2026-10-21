/*
 * A247795  Irregular triangle read by rows in which row n lists the parities
 *          of the divisors of n.
 * https://oeis.org/A247795
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   a(n) = parity (least significant bit) of the n‑th element of the
 *            irregular triangle read row by row, where each row n lists the
 *            parities of the divisors of n (1 for odd, 0 for even)
 * Range:     u8 n <= 7069, u16 n <= 7069, u32 n <= 7069, u64 n <= 7069,
 *            u128 n <= 7069, big any n
 * Helpers:   build_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The algorithm:
   1. Find the row m such that the cumulative number of divisors up to
      m‑1 is < n ≤ cumulative up to m.
   2. Let k = n – cumulative[m‑1] (1‑based position inside row m).
   3. Generate all divisors of m, sort them, pick the k‑th divisor d.
   4. Return d mod 2. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_spf.h"

static idx_t current_limit = 0;      /* highest n for which data is built */
static idx_t *spf_arr = NULL;        /* smallest prime factor array */
static uint32_t *tau_arr = NULL;     /* divisor counts τ(n) */
static unsigned long long *cum_arr = NULL; /* cumulative divisor counts */

/* Ensure data up to 'limit' (inclusive) is available. */
static void ensure_arrays(idx_t limit)
{
    if (limit <= current_limit)
        return;

    /* free old data if any */
    free(spf_arr);
    free(tau_arr);
    free(cum_arr);

    /* Build smallest prime factor up to limit */
    build_spf(limit, &spf_arr);               /* spf_arr[0] unused, spf_arr[1] = 0 */

    tau_arr = (uint32_t*)malloc((limit + 1) * sizeof(uint32_t));
    cum_arr = (unsigned long long*)malloc((limit + 1) * sizeof(unsigned long long));
    if (!tau_arr || !cum_arr)
        exit(EXIT_FAILURE);

    tau_arr[0] = 0;
    tau_arr[1] = 1;
    cum_arr[0] = 0;
    cum_arr[1] = 1;
    for (idx_t i = 2; i <= limit; ++i) {
        idx_t x = i;
        unsigned long long result = 1;
        while (x > 1) {
            idx_t p = spf_arr[x];
            unsigned exp = 0;
            while (x % p == 0) {
                x /= p;
                ++exp;
            }
            result *= (exp + 1);
        }
        tau_arr[i] = (uint32_t)result;
        cum_arr[i] = cum_arr[i-1] + result;
    }
    current_limit = limit;
}

/* Comparator for qsort of idx_t values. */
static int cmp_idx_t(const void *a, const void *b)
{
    idx_t va = *(const idx_t*)a;
    idx_t vb = *(const idx_t*)b;
    return (va < vb) ? -1 : (va > vb) ? 1 : 0;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                /* undefined but guard */

    /* Ensure data supports this index (row index ≤ n). */
    ensure_arrays(n);

    /* Binary search for minimal m with cum_arr[m] ≥ n. */
    idx_t lo = 1, hi = current_limit;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if (cum_arr[mid] >= (unsigned long long)n)
            hi = mid;
        else
            lo = mid + 1;
    }
    idx_t m = lo;
    unsigned long long cum_before = cum_arr[m-1];
    idx_t k = (idx_t)((unsigned long long)n - cum_before);    /* 1‑based position */

    /* Factor m using spf_arr. */
    idx_t x = m;
    idx_t primes[64];
    unsigned exps[64];
    size_t pf_cnt = 0;
    while (x > 1) {
        idx_t p = spf_arr[x];
        unsigned exp = 0;
        while (x % p == 0) {
            x /= p;
            ++exp;
        }
        primes[pf_cnt] = p;
        exps[pf_cnt] = exp;
        ++pf_cnt;
    }

    /* Compute total number of divisors to allocate array. */
    size_t total_div = 1;
    for (size_t i = 0; i < pf_cnt; ++i)
        total_div *= (exps[i] + 1);

    /* Generate all divisors. */
    idx_t *divs = (idx_t*)malloc(total_div * sizeof(idx_t));
    if (!divs)
        exit(EXIT_FAILURE);
    size_t div_cnt = 1;
    divs[0] = 1;
    for (size_t i = 0; i < pf_cnt; ++i) {
        size_t old_cnt = div_cnt;
        idx_t p = primes[i];
        unsigned exp = exps[i];
        idx_t mult = 1;
        for (unsigned e = 1; e <= exp; ++e) {
            mult *= p;
            for (size_t j = 0; j < old_cnt; ++j)
                divs[div_cnt++] = divs[j] * mult;
        }
    }

    /* Sort divisors. */
    qsort(divs, div_cnt, sizeof(idx_t), cmp_idx_t);

    /* Retrieve k-th divisor and its parity. */
    idx_t d = divs[k-1];
    free(divs);

    return (seq_t)(d & 1ULL);
}
