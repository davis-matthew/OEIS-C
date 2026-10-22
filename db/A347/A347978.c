/*
 * A347978  Expansion of e.g.f. 1/(1 + x)^(1/(1 - x)).
 * https://oeis.org/A347978
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = - Sum_{k=1..n} binom(n-1,k-1) * A024167(k) * a(n-k) *
 *            where A024167(k) = - k! * Sum_{j=1..k} (-1)^j / j *
 *            (equivalently, A024167(k) = Sum_{j=1..k} (-1)^{j+1} * k!/j). *
 *            a(0) = 1 * * The function below uses this recurrence with an
 *            on‑the‑fly computation of * the integer sequence A024167(k)....
 * Range:     i8 n <= 5, i16 n <= 9, i32 n <= 13, i64 n <= 21, i128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>          /* for idx_t */
#include <stdint.h>          /* for uint64_t */

seq_t a(idx_t n)
{
    /* static cache: a[0..max_n], b[0..max_n] (A024167 values), factorial (m-1)! */
    static seq_t a_arr[10001] = {0};
    static seq_t b_arr[10001] = {0};
    static seq_t fact = 1;          /* represents (m-1)! while computing a_m */
    static idx_t max_computed = 0;  /* largest index already filled */

    /* Handle trivial case */
    if (n == 0)
    {
        a_arr[0] = 1;
        return a_arr[0];
    }

    /* Ensure a_arr[0] is initialized */
    if (max_computed < 0)
        a_arr[0] = 1;

    if (n <= max_computed)
        return a_arr[n];

    /* Compute sequentially from max_computed+1 up to n */
    for (idx_t m = max_computed + 1; m <= n; ++m)
    {
        /* Compute A024167(m) = -m*b_{m-1} + (m-1)!*(-1)^m   */
        seq_t sign = (m & 1) ? -1 : 1;        /* (-1)^m */
        seq_t term = fact * sign;             /* (m-1)! * (-1)^m */
        b_arr[m] = (seq_t)m * b_arr[m - 1] - term;

        /* Compute a_m via recurrence */
        seq_t sum = 0;
        seq_t binom = 1;                      /* binom(m-1,0) */
        for (idx_t k = 1; k <= m; ++k)
        {
            seq_t prod = binom * b_arr[k];
            prod = prod * a_arr[m - k];
            sum += prod;

            /* Update binom to binom(m-1,k) for next iteration */
            if (k < m)
                binom = binom * (m - k) / k;
        }
        a_arr[m] = -sum;

        /* Update factorial to m! for next iteration */
        fact = fact * (seq_t)m;
    }

    max_computed = n;
    return a_arr[n];
}
