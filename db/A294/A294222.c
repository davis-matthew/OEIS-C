/*
 * A294222  Exponential transform of the Lucas numbers (A000204).
 * https://oeis.org/A294222
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=0}^{n-1} binom(n-1,k) * a(k) * L(n-k) (exponential
 *            transform of Lucas)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 21, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* size_t */
#include <stdlib.h>   /* malloc, realloc */

/* cached Lucas numbers (L(0)=2, L(1)=1, L(k)=L(k-1)+L(k-2)) */
static seq_t *lucas_arr = NULL;
static size_t lucas_len = 0;
static size_t lucas_cap = 0;

/* cached values of the sequence a(n) */
static seq_t *a_arr   = NULL;
static size_t a_len   = 0;
static size_t a_cap   = 0;

/* enlarge a dynamic array if needed */
static void ensure_capacity(size_t needed, seq_t **arr, size_t *cap)
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap == 0) ? 1 : *cap * 2;
    while (new_cap < needed) new_cap *= 2;
    *arr = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
    *cap = new_cap;
}

/* fill lucas_arr up to index n (inclusive) */
static void ensure_lucas(size_t n)
{
    if (lucas_len > n) return;
    if (lucas_len < 2) {
        /* initialise first two Lucas numbers */
        ensure_capacity(2, &lucas_arr, &lucas_cap);
        lucas_arr[0] = 2;
        lucas_arr[1] = 1;
        lucas_len = 2;
    }
    while (lucas_len <= n) {
        seq_t next = lucas_arr[lucas_len - 1] + lucas_arr[lucas_len - 2];
        ensure_capacity(lucas_len + 1, &lucas_arr, &lucas_cap);
        lucas_arr[lucas_len] = next;
        ++lucas_len;
    }
}

/* compute a(k) for all k <= n, storing in a_arr */
static void ensure_a(size_t n)
{
    if (a_len > n) return;
    if (a_len == 0) {
        ensure_capacity(1, &a_arr, &a_cap);
        a_arr[0] = 1;   /* a(0) = 1 */
        a_len = 1;
    }
    for (size_t k = a_len; k <= n; ++k) {
        ensure_lucas(k);
        seq_t sum = 0;
        seq_t c   = 1;   /* binom(k-1,0) */
        for (size_t j = 0; j < k; ++j) {
            /* contribution: a(j) * L(k-j) * c */
            seq_t contrib = a_arr[j];
            contrib = contrib * lucas_arr[k - j];
            contrib = contrib * c;
            sum += contrib;
            if (j + 1 < k) {
                seq_t numerator   = c * (seq_t)(k - 1 - j);
                seq_t denominator = (seq_t)(j + 1);
                c = numerator / denominator;   /* exact division */
            }
        }
        ensure_capacity(k + 1, &a_arr, &a_cap);
        a_arr[k] = sum;
    }
    a_len = n + 1;
}

/* external function required by the exercise */
seq_t a(idx_t n)
{
    if (n == (idx_t)~0) return (seq_t)0;   /* guard against unsigned wrap */
    ensure_a((size_t)n);
    return a_arr[(size_t)n];
}
