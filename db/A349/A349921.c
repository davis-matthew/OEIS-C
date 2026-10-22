/*
 * A349921  Dirichlet g.f.: Product_{k>=2} 1 / (1 - 2 * k^(-s)).
 * https://oeis.org/A349921
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the weighted count of unordered factorizations of n into
 *            integers ≥2, each such factorization weighted by 2^k where k is
 *            the number of factors
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

/* Helper structure for storing divisors and memoization of a(n,min_factor) values. */
typedef struct DivInfo {
    idx_t *divs;          /* sorted list of divisors of n */
    size_t divc;          /* number of divisors */
    seq_t *memo;          /* memoized results for each divisor position */
} DivInfo;

/* Global array of DivInfo, indexed by n. */
static DivInfo *divinfo = NULL;
static size_t divinfo_cap = 0;

/* Comparator for qsort on idx_t arrays. */
static int cmp_idx_t(const void *a, const void *b)
{
    idx_t v1 = *(const idx_t *)a;
    idx_t v2 = *(const idx_t *)b;
    return (v1 > v2) - (v1 < v2);
}

/* Ensure divinfo can hold entries up to at least n. */
static void ensure_divinfo(idx_t n)
{
    if ((size_t)n >= divinfo_cap) {
        size_t new_cap = n + 1;
        DivInfo *new_arr = (DivInfo *)realloc(divinfo, new_cap * sizeof(DivInfo));
        if (!new_arr) abort(); /* out of memory */
        /* initialize the new entries */
        for (size_t i = divinfo_cap; i < new_cap; ++i) {
            new_arr[i].divs = NULL;
            new_arr[i].divc = 0;
            new_arr[i].memo = NULL;
        }
        divinfo_cap = new_cap;
        divinfo = new_arr;
    }
}

/* Compute and store divisors of n (and memo array) if not already done. */
static DivInfo *get_divinfo(idx_t n)
{
    ensure_divinfo(n);
    if (divinfo[n].divs == NULL) {
        /* Count divisors. */
        size_t cnt = 0;
        for (idx_t d = 1; (unsigned long long)d * d <= (unsigned long long)n; ++d) {
            if (n % d == 0) {
                if (d == n / d) cnt += 1;
                else            cnt += 2;
            }
        }

        /* Allocate array of divisors. */
        idx_t *arr = (idx_t *)malloc(cnt * sizeof(idx_t));
        if (!arr) abort();
        size_t pos = 0;
        for (idx_t d = 1; (unsigned long long)d * d <= (unsigned long long)n; ++d) {
            if (n % d == 0) {
                arr[pos++] = d;
                if (d != n / d) arr[pos++] = n / d;
            }
        }
        qsort(arr, cnt, sizeof(idx_t), cmp_idx_t);

        /* Allocate memoization array, initialized to 0. */
        seq_t *memo = (seq_t *)calloc(cnt, sizeof(seq_t));
        if (!memo) abort();

        divinfo[n].divs = arr;
        divinfo[n].divc = cnt;
        divinfo[n].memo = memo;
    }
    return &divinfo[n];
}

/* Recursive computation of a(n,min_factor). */
static seq_t compute_a_rec(idx_t n, idx_t min_factor)
{
    if (n == 1) return (seq_t)1;          /* base case */
    if (min_factor > n) return (seq_t)0;  /* impossible to factor */

    DivInfo *info = get_divinfo(n);
    /* Find the first divisor >= min_factor. */
    size_t idx = 0;
    while (idx < info->divc && info->divs[idx] < min_factor) ++idx;
    if (idx == info->divc) return (seq_t)0;   /* no valid divisor */

    if (info->memo[idx] != (seq_t)0) return info->memo[idx];

    seq_t result = (seq_t)0;
    for (size_t j = idx; j < info->divc; ++j) {
        idx_t d = info->divs[j];
        if (d < (idx_t)2) continue;          /* factors must be ≥ 2 */
        seq_t sub = compute_a_rec(n / d, d);
        result += ((seq_t)2) * sub;
    }
    info->memo[idx] = result;
    return result;
}

/* Public function required by the framework. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined, safe fallback */
    if (n == 1) return (seq_t)1;
    return compute_a_rec(n, (idx_t)2);
}
