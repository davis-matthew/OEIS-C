/*
 * A383963  Irregular triangle read by rows: T(n,k) is the sum of the k-th
 *          pair of conjugate divisors of n. If n is a square then the central
 *          term in the row n is equal to 2*sqrt(n), with n >= 1, 1 <= k <=
 *          A000005(n).
 * https://oeis.org/A383963
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the n-th term in the flattened irregular triangle whose
 *            * entry T(m,k) for row m and position k is m/k + k, i.e. the sum
 *            of the k-th * pair of conjugate divisors of m (divisors listed
 *            in increasing order)
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* static state shared between calls */
static size_t max_precomputed = 0;   /* number of rows already prepared */
static size_t alloc_cap = 0;          /* current allocation capacity for arrays */
static seq_t *tau_arr   = NULL;       /* tau_arr[i] = number of divisor pairs of i */
static seq_t *pref_arr  = NULL;       /* pref_arr[i] = Σ_{j=1}^{i} tau_arr[j] */

/* Grow dynamic arrays if needed */
static void grow_arrays(size_t new_cap) {
    if (new_cap <= alloc_cap)
        return;
    size_t new_size = new_cap + 1; /* include index 0 */
    tau_arr = (seq_t *)realloc(tau_arr, new_size * sizeof(seq_t));
    pref_arr = (seq_t *)realloc(pref_arr, new_size * sizeof(seq_t));
    alloc_cap = new_cap;
}

/* Ensure data for all rows up to at least those needed to cover index `need` */
static void ensure_precomputed(idx_t need) {
    /* compute rows until cumulative count reaches `need` */
    while (pref_arr[max_precomputed] < need) {
        size_t new_row = max_precomputed + 1;
        /* allocate more space if necessary */
        if (new_row > alloc_cap)
            grow_arrays((alloc_cap ? alloc_cap : 8) * 2);
        /* compute tau(new_row) */
        seq_t tau_val = 1;
        idx_t n = new_row;
        for (idx_t p = 2; p * p <= n; ++p) {
            if (n % p == 0) {
                unsigned int e = 0;
                while (n % p == 0) {
                    n /= p;
                    ++e;
                }
                tau_val *= (seq_t)(e + 1);
            }
        }
        if (n > 1)
            tau_val *= 2;  /* remaining prime factor */
        tau_arr[new_row] = tau_val;
        pref_arr[new_row] = pref_arr[new_row - 1] + tau_val;
        ++max_precomputed;
    }
}

/* Binary search to find first row with pref_arr[row] >= target */
static size_t find_row(idx_t target) {
    size_t lo = 1, hi = max_precomputed;
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        if (pref_arr[mid] >= target)
            hi = mid;
        else
            lo = mid + 1;
    }
    return lo;
}

/* Public function required by the build system */
seq_t a(idx_t n) {
    if (n == 0)
        return 0; /* out‑of‑range guard */

    /* Ensure we have enough precomputed rows to cover index n */
    if (max_precomputed == 0) {
        /* initial allocation of 16 rows (index 0 unused) */
        grow_arrays(16);
        /* set pref_arr[0] = 0 implicitly after realloc */
        pref_arr[0] = 0;
        max_precomputed = 0;
    }
    ensure_precomputed(n);

    /* locate row that contains the n-th term */
    size_t row = find_row((idx_t)n);
    /* zero‑based position within that row */
    idx_t pos = (idx_t)n - (idx_t)pref_arr[row - 1] - 1;

    /* find the (pos)-th divisor of `row` in ascending order */
    idx_t count = 0;
    for (idx_t d = 1; d <= row; ++d) {
        if (row % d == 0) {
            if (count == pos) {
                /* compute sum of conjugate pair for divisor d */
                return (seq_t)d + (seq_t)(row / d);
            }
            ++count;
        }
    }
    /* should never reach here */
    return 0;
}
