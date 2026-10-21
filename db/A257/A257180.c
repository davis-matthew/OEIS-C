/*
 * A257180  Triangle, read by rows, T(n,k) = t(n-k, k) where t(n,m) =
 *          f(m)*t(n-1,m) + f(n)*t(n,m-1), and f(x) = x + 3.
 * https://oeis.org/A257180
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   * a(n) = T(n, k) read by rows, where * T(n, k) = t(n-k, k) *
 *            t(0,0) = 1 * t(n,m) = 0 if n<0 or m<0 * else * t(n,m) =
 *            (m+3)*t(n-1,m) + (n+3)*t(n,m-1) * * a(n) is the entry in the
 *            anti‑diagonal order of the triangle * defined by T. For example:
 *            * a(0) = 1 * a(1) = 3, a(2) = 3 * a(3) = 9, a(4) = 24,...
 * Range:     u8 n <= 10, u16 n <= 22, u32 n <= 68, u64 n <= 177,
 *            u128 n <= 509, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* cache for values of t(n,m)  (square table).  */
    static seq_t *t = NULL;
    static idx_t t_cap = 0;          /* current side length of the square table */

    /* Find the row r and column c such that a(n) = t(r-c, c) */
    idx_t r = (idx_t)((sqrt(8.0 * (double)n + 1.0) - 1.0) / 2.0);
    /* Adjust in case of rounding errors */
    while ((r + 1) * (r + 2) / 2 <= n) ++r;
    while (r * (r + 1) / 2 > n) --r;
    idx_t c = n - r * (r + 1) / 2;

    /* Ensure the cache covers row r */
    idx_t needed = r + 1;
    if (t_cap < needed) {
        idx_t newcap = needed;
        seq_t *newt = (seq_t *)malloc(newcap * newcap * sizeof(seq_t));
        if (!newt) return 0;   /* allocation failed, return 0 as a safe fallback */

        /* Build the entire square table up to newcap-1   */
        for (idx_t i = 0; i < newcap; ++i) {
            for (idx_t j = 0; j < newcap; ++j) {
                if (i == 0 && j == 0) {
                    newt[0] = (seq_t)1;
                } else {
                    seq_t val = 0;
                    if (i > 0) {
                        val += ((seq_t)(j + 3)) * newt[(i - 1) * newcap + j];
                    }
                    if (j > 0) {
                        val += ((seq_t)(i + 3)) * newt[i * newcap + (j - 1)];
                    }
                    newt[i * newcap + j] = val;
                }
            }
        }
        free(t);
        t = newt;
        t_cap = newcap;
    }

    /* Retrieve t(r-c, c) */
    idx_t row_minus_col = r - c;
    size_t idx = (size_t)row_minus_col * (size_t)t_cap + (size_t)c;
    return t[idx];
}
