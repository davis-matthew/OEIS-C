/*
 * A346856  Number of partitions of the (n+10)-multiset {0,...,0,1,2,...,n}
 *          with ten 0's.
 * https://oeis.org/A346856
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=1}^{n} S(n,k) * f(k) (n > 0) a(0) = P(10) where
 *            S(n,k) are Stirling numbers of the second kind, P(m) is the
 *            partition number of m, and f(k) = sum_{m=0}^{10} C(k+9-m, k-1) *
 *            P(m). The values are computed with exact integer arithmetic for
 *            the types seq_t and idx_t supplied by the build
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 20, u128 n <= 37,
 *            big any n
 * Helpers:   allocate_seq_array, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/allocate_seq_array.h"
#include "../utils/seq_realloc.h"

/* Pre‑computed partition numbers P(m) for m = 0 .. 10 */
static const seq_t PARTS[11] = {
    1U,  /* P(0) */
    1U,  /* P(1) */
    2U,  /* P(2) */
    3U,  /* P(3) */
    5U,  /* P(4) */
    7U,  /* P(5) */
    11U, /* P(6) */
    15U, /* P(7) */
    22U, /* P(8) */
    30U, /* P(9) */
    42U  /* P(10) */
};

/* ----------  f(k)  ---------- */
static seq_t *f_arr      = NULL;   /* f_arr[1 .. n]  (index 0 unused) */
static size_t f_cap      = 0;      /* allocated capacity of f_arr   */
static idx_t  f_last_k   = 0;      /* largest k for which f_arr[k] is valid */
static seq_t  h[11];              /* helper values: h[m] = C(k+9-m, k-1) */

/* Initialise helper array h for k = 1 */
static void init_h(void) {
    for (idx_t m = 0; m <= 10; ++m) h[m] = 1;   /* C(1+m-1, m) = 1 */
}

/* Ensure f_arr is large enough */
static void f_expand(size_t needed) {
    if (needed <= f_cap) return;
    size_t new_cap = f_cap ? f_cap * 2 : needed;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *new_arr = seq_realloc(f_arr, f_cap, new_cap);
    if (new_arr == NULL) *new_arr = NULL; /* will trigger abort below */
    if (new_arr == NULL) abort();
    /* Initialise new region to zero */
    memset(new_arr + f_cap, 0, (new_cap - f_cap) * sizeof(seq_t));
    f_arr = new_arr;
    f_cap = new_cap;
}

/* Compute f(k) for all k up to n */
static void ensure_f_up_to(idx_t n) {
    if (f_last_k == 0) init_h();   /* first time */
    while (f_last_k < n) {
        idx_t k = f_last_k + 1;
        /* compute f(k) = sum_{m=0}^{10} h[m] * PARTS[m] */
        seq_t sum = 0;
        for (idx_t m = 0; m <= 10; ++m) {
            sum += h[m] * PARTS[m];
        }
        f_expand(k + 1);
        f_arr[k] = sum;

        /* update h to represent k+1 */
        for (idx_t m = 0; m <= 10; ++m) {
            /* h_new = h_old * (k+10-m) / k */
            seq_t num = h[m] * (seq_t)(k + 10 - m);
            h[m] = num / (seq_t)k;   /* exact division */
        }
        f_last_k = k;
    }
}

/* ----------  Stirling numbers  ---------- */
static seq_t *S_prev   = NULL;   /* S(k-1, *)  */
static seq_t *S_curr   = NULL;   /* S(k,   *)  */
static size_t S_cap   = 0;      /* allocated capacity */

/* Ensure S_prev and S_curr are large enough for size >= needed */
static void S_expand(size_t needed) {
    if (needed <= S_cap) return;
    size_t new_cap = S_cap ? S_cap * 2 : needed;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *new_prev = seq_realloc(S_prev, S_cap, new_cap);
    seq_t *new_curr = seq_realloc(S_curr, S_cap, new_cap);
    if (new_prev == NULL || new_curr == NULL) abort();
    /* Initialise new region to zero */
    memset(new_prev + S_cap, 0, (new_cap - S_cap) * sizeof(seq_t));
    memset(new_curr + S_cap, 0, (new_cap - S_cap) * sizeof(seq_t));
    S_prev = new_prev;
    S_curr = new_curr;
    S_cap = new_cap;
}

/* ----------  a(n) values  ---------- */
static seq_t *a_arr     = NULL;   /* a[0 .. n] */
static size_t a_cap     = 0;     /* allocated capacity */
static idx_t  a_max     = 0;     /* largest n already computed */

/* Ensure a_arr is large enough */
static void a_expand(size_t needed) {
    if (needed <= a_cap) return;
    size_t new_cap = a_cap ? a_cap * 2 : needed;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *new_arr = seq_realloc(a_arr, a_cap, new_cap);
    if (new_arr == NULL) abort();
    memset(new_arr + a_cap, 0, (new_cap - a_cap) * sizeof(seq_t));
    a_arr = new_arr;
    a_cap = new_cap;
}

/* Compute a(n) for all n up to the requested value */
static void ensure_a_up_to(idx_t n) {
    if (n <= a_max) return;
    size_t needed = (size_t)n + 1;
    a_expand(needed);

    /* Initialise a(0) */
    if (a_max == 0) {
        a_arr[0] = PARTS[10];      /* P(10) = 42 */
        a_expand(1);
        S_expand(1);
        S_prev[0] = 1;            /* S(0,0) = 1 */
    }

    /* Ensure f(k) values exist up to n */
    ensure_f_up_to(n);

    /* Compute a(curr) for curr = a_max+1 .. n */
    for (idx_t curr = a_max + 1; curr <= n; ++curr) {
        S_expand((size_t)curr + 1);

        /* Build current Stirling row */
        S_curr[0] = 0;
        for (idx_t k = 1; k <= curr; ++k) {
            seq_t term1 = S_prev[k];
            seq_t term2 = S_prev[k - 1];
            S_curr[k] = (seq_t)k * term1 + term2;
        }

        /* Compute a[curr] = Σ_{k=1}^{curr} S_curr[k] * f_arr[k] */
        seq_t sum = 0;
        for (idx_t k = 1; k <= curr; ++k) {
            sum += S_curr[k] * f_arr[k];
        }
        a_arr[curr] = sum;

        /* Prepare for next iteration */
        seq_t *tmp = S_prev;
        S_prev   = S_curr;
        S_curr   = tmp;
    }
    a_max = n;
}

seq_t a(idx_t n) {
    if (n == 0) return PARTS[10];   /* a(0) = 42 */
    ensure_a_up_to(n);
    return a_arr[n];
}
