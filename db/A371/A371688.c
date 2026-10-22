/*
 * A371688  Triangle read by rows: T(n, k) = (2*n + 1)! * [y^(2*k)]
 *          [x^(2*n+1)] arctan(sec(x*y)*sinh(x)).
 * https://oeis.org/A371688
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = T(r,c) where T(n,k)=(-1)^k*binomial(2*n+1,2*k)*Euler(2*n)
 *            and the triangle is read row‑wise: r = ⌊(√(8n+1)-1)/2⌋, c = n -
 *            r(r+1)/2
 * Range:     i8 n <= 6, i16 n <= 10, i32 n <= 23, i64 n <= 27, i128 n <= 27,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/binomial.h"

static seq_t *euler_table = NULL;
static size_t euler_count = 0;   /* number of computed Euler(2k) values */

/* Grow the Euler table to at least index n and compute missing values. */
static void grow_euler(idx_t n)
{
    if ((size_t)n >= euler_count) {
        size_t old = euler_count;
        size_t newsize = (size_t)n + 1;
#if __cplusplus
        seq_t *tmp = new seq_t[newsize];
        for (size_t i = 0; i < old; ++i)
            tmp[i] = euler_table[i];
        delete[] euler_table;
        euler_table = tmp;
#else
        seq_t *tmp = (seq_t*)realloc(euler_table, newsize * sizeof(seq_t));
        if (!tmp) abort();
        euler_table = tmp;
#endif
        euler_count = newsize;

        /* Compute the missing Euler numbers using the recurrence
           E_{2m} = -∑_{k=0}^{m-1} binomial(2m,2k) * E_{2k}.   */
        for (size_t m = old; m <= (size_t)n; ++m) {
            if (m == 0) {
                euler_table[0] = (seq_t)1;   /* E_0 = 1 */
            } else {
                seq_t sum = (seq_t)0;
                for (size_t k = 0; k < m; ++k) {
                    seq_t comb = binomial((idx_t)(2 * m), (idx_t)(2 * k));
                    sum += comb * euler_table[k];
                }
                euler_table[m] = -sum;
            }
        }
    }
}

/* Return Euler(2n). */
static inline seq_t get_euler_even(idx_t n)
{
    grow_euler(n);
    return euler_table[(size_t)n];
}

/* Main function required by the build system */
seq_t a(idx_t n)
{
    /* Find row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    unsigned long long disc = 8ull * n + 1;
    double sq = sqrt((double)disc);
    idx_t r = (idx_t)((sq - 1.0) / 2.0);
    /* Adjust in case of floating‑point rounding */
    while ((r + 1) * (r + 2) / 2 <= n) ++r;
    while (r * (r + 1) / 2 > n) --r;

    /* Column index within the row (0‑based) */
    idx_t first_in_row = r * (r + 1) / 2;
    idx_t c = n - first_in_row;               /* 0 <= c <= r */

    /* Compute the term T(r,c) = (-1)^c * binomial(2*r+1,2*c) * Euler(2*r) */
    seq_t euler_val = get_euler_even(r);
    idx_t N = 2 * r + 1;
    idx_t K = 2 * c;
    seq_t binom_val = binomial(N, K);

    seq_t result = euler_val * binom_val;
    if (c & 1) /* multiply by (-1)^c where c is odd */
        result = -result;

    return result;
}
