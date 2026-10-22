/*
 * A346157  Total number of left-to-right maxima in Dyck paths of semilength
 *          n.
 * https://oeis.org/A346157
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{r=1..n} (d(r+1)-d(r)) * * ( binomial(2*n-1, n-r) -
 *            binomial(2*n-1, n-r-1) ) * where d(k) is the number of divisors
 *            of k
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 15, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* ----  Cache for divisor counts  ---- */
static idx_t *divisors = NULL;
static size_t divisors_cap = 0;

/* Ensure divisor counts are precalculated up to 'limit' (inclusive). */
static void ensure_divisors(idx_t limit)
{
    if (limit <= (idx_t)divisors_cap) return;

    free(divisors);
    divisors = (idx_t *)calloc((size_t)(limit + 1), sizeof(idx_t));
    if (!divisors) abort();   /* OOM */

    for (idx_t i = 1; i <= limit; ++i) {
        for (idx_t j = i; j <= limit; j += i)
            divisors[j] += 1;
    }
    divisors_cap = (size_t)limit;
}

/* ----  Cache for binomial differences (C(2n-1,k)-C(2n-1,k-1))  ---- */
static seq_t *diff_arr = NULL;
static size_t diff_cap = 0;

static void ensure_diff_arr(size_t need)
{
    if (need <= diff_cap) return;
    size_t new_cap = diff_cap ? diff_cap << 1 : 2;
    if (new_cap < need) new_cap = need;
    seq_t *tmp = (seq_t *)realloc(diff_arr, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    diff_arr = tmp;
    diff_cap = new_cap;
}

/* Compute diff_arr[k] = C(2n-1, k) - C(2n-1, k-1), 0 <= k < n. */
static void compute_diffs(idx_t n)
{
    idx_t m = 2 * n - 1;
    seq_t c_prev = 1;          /* C(m,0) */
    diff_arr[0] = c_prev;      /* difference for k=0 is C(m,0) */

    for (idx_t k = 1; k < n; ++k) {
        seq_t num = (seq_t)(m - (k - 1));
        seq_t den = (seq_t)k;
        seq_t c_cur = c_prev * num / den;   /* C(m,k) */
        diff_arr[k] = c_cur - c_prev;      /* difference */
        c_prev = c_cur;
    }
}

/* ----  main sequence function  ---- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    /* divisor counts up to n+1, because we need d(r+1) with r=n. */
    ensure_divisors(n + 1);
    /* difference array length n. */
    ensure_diff_arr((size_t)n);
    compute_diffs(n);

    seq_t pos_sum = 0;
    seq_t neg_sum = 0;

    for (idx_t r = 1; r <= n; ++r) {
        /* signed difference of divisor counts */
        int64_t delta = (int64_t)divisors[r + 1] - (int64_t)divisors[r];
        idx_t k = n - r;                 /* k = n-r */
        seq_t part = diff_arr[k];        /* non‑negative */

        if (delta >= 0) {
            seq_t contribution = part;
            contribution *= (seq_t)delta;     /* multiply by positive delta */
            pos_sum += contribution;
        } else {
            seq_t contribution = part;
            contribution *= (seq_t)(-delta);  /* multiply by |delta| */
            neg_sum += contribution;
        }
    }

    seq_t result = pos_sum;
    result -= neg_sum;
    return result;
}
