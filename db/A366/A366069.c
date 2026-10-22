/*
 * A366069  The number of lit cells in weakly increasing partitions of n when
 *          light shines from the northwest. Here partitions are represented
 *          from left to right by columns of cells.
 * https://oeis.org/A366069
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=0}^{n} p(k) * (2 * tau(n-k) - 1), where p(k) is
 *            the number of partitions of k and tau(m) is the number of
 *            divisors of m
 * Range:     u8 n <= 9, u16 n <= 28, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

static idx_t max_n = 0;                  /* largest n computed so far   */
static seq_t *p_arr  = NULL;             /* partition numbers 0..max_n */
static seq_t *a_arr  = NULL;             /* sequence values  0..max_n */
static uint32_t *tau_arr = NULL;         /* divisor counts 0..max_n  */
static seq_t *s_arr  = NULL;             /* auxiliary coefficient 0..max_n */

/* Ensure that all arrays can hold indices up to n (inclusive). */
static void ensure_capacity(idx_t n)
{
    if (n <= max_n) return;

    /* Allocate or grow arrays */
    size_t new_cap = (size_t)n + 1;
    p_arr  = (seq_t*)realloc(p_arr,  new_cap * sizeof(seq_t));
    a_arr  = (seq_t*)realloc(a_arr,  new_cap * sizeof(seq_t));
    tau_arr = (uint32_t*)realloc(tau_arr, new_cap * sizeof(uint32_t));
    s_arr  = (seq_t*)realloc(s_arr,  new_cap * sizeof(seq_t));

    /* Compute divisor counts tau[i] for 1..n */
    for (idx_t i = 1; i <= n; ++i)
        tau_arr[i] = 0;   /* clear new part */
    /* recompute all tau up to n */
    for (idx_t i = 1; i <= n; ++i)
        for (idx_t j = i; j <= n; j += i)
            ++tau_arr[j];

    /* Auxiliary coefficients s[i] = 2 * tau[i] - 1, with s[0] = 0 */
    s_arr[0] = 0;
    for (idx_t i = 1; i <= n; ++i)
        s_arr[i] = (seq_t)2 * (seq_t)tau_arr[i] - 1;

    /* Partition numbers p[0..n] via recurrence of generalized pentagonal numbers */
    p_arr[0] = 1;
    for (idx_t m = 1; m <= n; ++m) {
        seq_t sum = 0;
        for (int k = 1; ; ++k) {
            idx_t g1 = (idx_t)k * (3 * k - 1) / 2;
            if (g1 > m) break;
            if (k & 1)
                sum += p_arr[m - g1];
            else
                sum -= p_arr[m - g1];

            idx_t g2 = (idx_t)k * (3 * k + 1) / 2;
            if (g2 <= m) {
                if (k & 1)
                    sum += p_arr[m - g2];
                else
                    sum -= p_arr[m - g2];
            }
        }
        p_arr[m] = sum;
    }

    /* Convolution to compute a[0..n] */
    a_arr[0] = 0;   /* a(0) undefined by OEIS; use 0 for safety */
    for (idx_t m = 1; m <= n; ++m) {
        seq_t acc = 0;
        for (idx_t k = 0; k <= m; ++k)
            acc += p_arr[k] * s_arr[m - k];
        a_arr[m] = acc;
    }

    max_n = n;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* defined for safety */
    ensure_capacity(n);
    return a_arr[n];
}
