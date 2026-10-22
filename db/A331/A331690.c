/*
 * A331690  a(n) = Sum_{k=0..n} Stirling2(n,k) * k! * n^(n - k).
 * https://oeis.org/A331690
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} Stirling2(n,k) * k! * n^(n - k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Static storage for computed values and Stirling* k! array. */
static seq_t *f_arr = NULL;      /* f_k = S(n,k)*k! for current n   */
static seq_t *a_arr = NULL;      /* cached a(n) values              */
static size_t f_cap = 0;         /* allocated capacity of f_arr     */
static size_t a_cap = 0;         /* allocated capacity of a_arr     */
static uint64_t cur_n = 0;       /* last n that has been computed */

/* Ensure arrays can hold indices up to n. */
static void ensure_cap(idx_t n)
{
    if (f_arr == NULL) {
        f_cap = 1;
        f_arr = (seq_t *)calloc(f_cap, sizeof(seq_t));
        if (!f_arr) exit(EXIT_FAILURE);
        f_arr[0] = 1;               /* S(0,0)*0! = 1            */
    }
    if (a_arr == NULL) {
        a_cap = 1;
        a_arr = (seq_t *)calloc(a_cap, sizeof(seq_t));
        if (!a_arr) exit(EXIT_FAILURE);
        a_arr[0] = 1;               /* a(0) = 1                 */
    }
    if ((size_t)n + 1 > f_cap) {
        size_t new_cap = ((size_t)n + 1) * 2;
        seq_t *tmp = (seq_t *)realloc(f_arr, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        f_arr = tmp;
        f_cap = new_cap;
    }
    if ((size_t)n + 1 > a_cap) {
        size_t new_cap = ((size_t)n + 1) * 2;
        seq_t *tmp = (seq_t *)realloc(a_arr, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        a_arr = tmp;
        a_cap = new_cap;
    }
}

/* Compute a(n) from the current f_arr (which holds values for that n). */
static void compute_a_for(idx_t n)
{
    seq_t sum = 0;
    seq_t pow = 1;
    for (idx_t k = n; ; --k) {
        sum += f_arr[k] * pow;
        if (k == 0) break;
        pow *= (seq_t)n;
    }
    a_arr[n] = sum;
}

/* Public function returning a(n).  It memoizes intermediate results. */
seq_t a(idx_t n)
{
    ensure_cap(n);
    while (n > cur_n) {
        idx_t old_n = cur_n;
        idx_t new_n = old_n + 1;
        /* Update f_arr in place from high to low indices. */
        for (idx_t k = new_n; k; --k) {
            seq_t term_before = f_arr[k - 1];
            seq_t term2 = (k <= old_n) ? f_arr[k] : (seq_t)0;
            seq_t val = (seq_t)k * (term_before + term2);
            f_arr[k] = val;
        }
        f_arr[0] = 0;           /* For n>0, S(n,0) = 0 */
        cur_n = new_n;
        compute_a_for(cur_n);
    }
    return a_arr[n];
}
