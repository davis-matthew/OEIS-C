/*
 * A382893  G.f. A(x) satisfies A(x) = 1/( 1 - x * (1+x)^2 * A(x) )^2.
 * https://oeis.org/A382893
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is defined by the algebraic * equation A(x) = 1 / (1 -
 *            x(1+x)^2 A(x))^2. * From this we obtain the recurrence * a(0) =
 *            1 * a(n) = 2·b(n) - c(n) (n ≥ 1) * where * b(n) = a2(n-1) +
 *            2·a2(n-2) + a2(n-3) * c(n) = Σ_{t=2..6} w_t · Σ_{i=0..n-t}
 *            a(i)·a2(n-t-i) * w = {1,4,6,4,1} corresponding to t =...
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>  /* for memset */

static seq_t *a_arr      = NULL;   /* a[0..max_n]   */
static seq_t *a2_arr     = NULL;   /* a2[0..2*max_n]*/
static size_t a_cap      = 0;       /* capacity of a_arr   */
static size_t a2_cap     = 0;       /* capacity of a2_arr  */
static idx_t   max_n     = 0;       /* largest index computed */

/* Ensure a_arr can hold indices up to n */
static void ensure_a_size(idx_t n)
{
    if (n < (idx_t)a_cap) return;
    size_t new_cap = a_cap ? a_cap : 16;
    while (new_cap <= (size_t)n) new_cap <<= 1;
    a_arr = (seq_t *)realloc(a_arr, new_cap * sizeof(seq_t));
    if (a_arr != NULL) {
        for (size_t i = a_cap; i < new_cap; ++i) a_arr[i] = 0;
    }
    a_cap = new_cap;
}

/* Ensure a2_arr can hold indices up to 2*n */
static void ensure_a2_size(idx_t n)
{
    size_t needed = 2 * (size_t)n + 1;
    if (needed <= a2_cap) return;
    size_t new_cap = a2_cap ? a2_cap : 64;
    while (new_cap < needed) new_cap <<= 1;
    a2_arr = (seq_t *)realloc(a2_arr, new_cap * sizeof(seq_t));
    if (a2_arr != NULL) {
        for (size_t i = a2_cap; i < new_cap; ++i) a2_arr[i] = 0;
    }
    a2_cap = new_cap;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* initialise seed if necessary */
    if (max_n == 0 && a_arr == NULL) {
        ensure_a_size(0);
        ensure_a2_size(0);
        a_arr[0] = 1;
        a2_arr[0] = 1;                /* a2[0] = a0*a0 */
        max_n = 0;
    }

    if (n <= max_n) return a_arr[n];

    /* grow arrays to accommodate new indices */
    ensure_a_size(n);
    ensure_a2_size(n);

    /* recurrence for n = max_n+1 .. n */
    for (idx_t cur = max_n + 1; cur <= n; ++cur)
    {
        /* compute b(cur) */
        seq_t b = 0;
        if (cur >= 1) b += a2_arr[cur - 1];
        if (cur >= 2) b += (seq_t)2 * a2_arr[cur - 2];
        if (cur >= 3) b += a2_arr[cur - 3];

        /* compute c(cur) */
        static const int w[5] = {1, 4, 6, 4, 1};
        seq_t c = 0;
        for (int t = 2; t <= 6; ++t)
        {
            if (cur < (idx_t)t) continue;
            idx_t m = cur - t;                   /* m = n - t  (≥ 0) */
            seq_t sum = 0;
            for (idx_t i = 0; i <= m; ++i)
                sum += a_arr[i] * a2_arr[m - i];
            c += (seq_t)w[t - 2] * sum;
        }

        /* a(cur) = 2*b(cur) - c(cur) */
        a_arr[cur] = (seq_t)2 * b - c;

        /* update convolution array a2_arr with new a[cur] */
        /* first: pairs where the second index is cur */
        for (idx_t i = 0; i <= cur; ++i)
            a2_arr[i + cur] += a_arr[i] * a_arr[cur];
        /* second: pairs where the first index is cur, second < cur */
        for (idx_t i = 0; i < cur; ++i)
            a2_arr[cur + i] += a_arr[cur] * a_arr[i];
    }

    max_n = n;
    return a_arr[n];
}
