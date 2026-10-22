/*
 * A338568  Number of integers less than n with the same number of unordered
 *          factorizations as n.
 * https://oeis.org/A338568
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |{j < n : A001055(j) = A001055(n)}|
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A001055 (unordered factorization counts) by recursion
   and keeps a running frequency table of those counts. */
#include <stdlib.h>

/* static state across calls */
static seq_t *factor_counts = NULL;      /* A001055 values, 1‑based */
static seq_t *freq          = NULL;      /* frequency of each A001055 value */
static idx_t  max_n_processed = 0;      /* largest n for which we have computed */
static size_t freq_cap = 0;             /* capacity of freq array */

/* ensure freq has space for index `cnt` (0‑based) */
static void ensure_freq_cap(seq_t cnt)
{
    size_t idx = (size_t)cnt;            /* cnt is expected to be small */
    if (idx + 1 <= freq_cap) return;
    size_t new_cap = freq_cap ? freq_cap * 2 : 16;
    if (idx + 1 > new_cap) new_cap = idx + 1;
    seq_t *tmp = (seq_t*)realloc(freq, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    for (size_t i = freq_cap; i < new_cap; ++i) tmp[i] = 0;
    freq = tmp;
    freq_cap = new_cap;
}

/* recursive count of unordered factorizations of n
   with all factors >= min_factor. */
static seq_t count_upto(idx_t n, idx_t min_factor)
{
    seq_t total = 1;                     /* the factorization consisting of n itself */
    for (idx_t d = min_factor; d * d <= n; ++d) {
        if (n % d == 0) {
            total += count_upto(n / d, d);
        }
    }
    return total;
}

/* compute A001055 values up to n (inclusive) */
static void compute_counts_up_to(idx_t n)
{
    if (n <= max_n_processed) return;

    size_t new_cap = (size_t)n + 1;
    if (!factor_counts) {
        factor_counts = (seq_t*)calloc(new_cap, sizeof(seq_t));
    } else {
        seq_t *tmp = (seq_t*)realloc(factor_counts, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        for (size_t i = max_n_processed + 1; i < new_cap; ++i) tmp[i] = 0;
        factor_counts = tmp;
    }

    for (idx_t m = max_n_processed + 1; m <= n; ++m) {
        seq_t cnt;
        if (m == 1) {
            cnt = 1;                     /* A001055(1) = 1 */
        } else {
            cnt = count_upto(m, 2);
        }
        factor_counts[m] = cnt;
        ensure_freq_cap(cnt);
        size_t idx = (size_t)cnt;
        freq[idx] += 1;
    }

    max_n_processed = n;
}

/* required function: number of j < n with same A001055 value as n */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;         /* undefined for n=0, avoid UB */
    compute_counts_up_to(n);
    seq_t cnt = factor_counts[n];
    size_t idx = (size_t)cnt;
    seq_t frq   = freq[idx];
    return frq - 1;                      /* exclude n itself */
}
