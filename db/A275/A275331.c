/*
 * A275331  Triangle read by rows, T(n,k) = k*Sum_{m=1..n/k} t(k)*t(n-k*m+1)
 *          with t = A000081, for n>=1 and 1<=k<=n.
 * https://oeis.org/A275331
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where the triangle T is read by rows;
 *            T(r,k)=k*sum_{m=1..floor(r/k)} t(k)*t(r-k*m+1). t(n) is the
 *            number of unlabeled rooted trees with n nodes (A000081)
 * Range:     u8 n <= 21, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS A275331. */
#include <stdlib.h>
#include <stddef.h>

static seq_t *t_cache = NULL;   /* 1-based array of t(n), t_cache[0] unused */
static idx_t t_cache_max = 0;   /* largest n for which t_cache has been built */

static void compute_t_upto(idx_t limit)
{
    if (limit <= t_cache_max)
        return;               /* already computed */

    size_t new_size = (size_t)limit + 1;
    seq_t *tmp = (seq_t *)realloc(t_cache, new_size * sizeof(seq_t));
    if (!tmp) abort();         /* allocation failure */
    t_cache = tmp;

    if (t_cache_max == 0) {    /* first time: initialise index 1 */
        t_cache[0] = 0;
        t_cache[1] = 1;        /* t(1) = 1 */
        t_cache_max = 1;
    }

    for (idx_t n = t_cache_max + 1; n <= limit; ++n) {
        seq_t sum = 0;
        for (idx_t k = 1; k <= n - 1; ++k) {
            /* divisor sum Σ_{d|k} d*t(d) */
            seq_t div_sum = 0;
            for (idx_t d = 1; d <= k; ++d) {
                if (k % d == 0)
                    div_sum += (seq_t)d * t_cache[d];
            }
            sum += div_sum * t_cache[n - k];
        }
        t_cache[n] = sum / ((seq_t)n - 1);
    }
    t_cache_max = limit;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Find the row r such that r(r+1)/2 >= n. */
    idx_t r = 0;
    idx_t acc = 0;
    while (acc < n) {
        ++r;
        acc += r;
    }
    idx_t prev = acc - r;      /* elements before this row */
    idx_t k = n - prev;        /* column within the row (1-based) */

    compute_t_upto(r);

    seq_t sum_m = 0;
    idx_t max_m = r / k;      /* floor(r/k) */
    for (idx_t m = 1; m <= max_m; ++m) {
        sum_m += t_cache[k] * t_cache[r - k * m + 1];
    }
    return (seq_t)k * sum_m;
}
