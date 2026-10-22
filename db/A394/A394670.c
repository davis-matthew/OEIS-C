/*
 * A394670  Triangle read by rows: T(n,k) is the number of ways n students
 *          take an exam one by one on any number of consecutive days as long
 *          as there are at least k students for each day.
 * https://oeis.org/A394670
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = T(r,k) where (r,k) is the nth term read by rows of * the
 *            triangle T(n,k) = n! * [x^n] 1/(1 - x^k/(1 - x)). * r =
 *            floor((sqrt(8*n+1)-1)/2), * k = n - r*(r+1)/2. * For k==0 the
 *            value is 0, except for r==0 where T(0,0)=1
 * Range:     u8 n <= 15, u16 n <= 28, u32 n <= 66, u64 n <= 153,
 *            u128 n <= 435, big any n
 * Helpers:   binomial, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/factorial.h"   /* seq_t factorial(idx_t n)       */
#include "../utils/binomial.h"    /* static seq_t binomial(idx_t n, idx_t k) */

static void
idx_to_row_col(idx_t n, idx_t *row, idx_t *col)
{
    if (n == 0) {
        *row = 0;
        *col = 0;
        return;
    }
    /* Compute r = floor((sqrt(8*n+1)-1)/2) using long double to avoid
     * integer overflow for moderately sized n (here n <= 10^4). */
    long double d = sqrtl((long double)n * 8.0L + 1.0L);
    idx_t r = (idx_t)((d - 1.0L) / 2.0L);

    /* Adjust r to satisfy r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    while ((unsigned __int128)r * (r + 1) / 2 > n)
        --r;
    while ((unsigned __int128)(r + 1) * (r + 2) / 2 <= n)
        ++r;

    *row = r;
    *col = n - (unsigned __int128)r * (r + 1) / 2;
}

seq_t a(idx_t n)
{
    idx_t r, k;
    idx_to_row_col(n, &r, &k);

    /* Edge cases */
    if (r == 0)          /* T(0,0) = 1 */
        return (seq_t)1;
    if (k == 0)          /* first element of a non‑zero row: 0 */
        return (seq_t)0;

    /* Compute the sum of binomial terms */
    seq_t sum = (seq_t)0;
    idx_t m_max = r / k;   /* floor(r / k) */
    for (idx_t m = 1; m <= m_max; ++m) {
        /* a = r - (k-1)*m - 1,  b = m - 1 */
        idx_t a_par = r - (k - 1) * m - 1;
        idx_t b = m - 1;
        /* binomial(a_par, b) is defined in binomial.h */
        seq_t term = binomial(a_par, b);
        sum += term;
    }

    /* Multiply by r! (factorial).  We compute it directly to avoid
     * allocating a cache; r <= 10^4 for the tests, so the loop is fast. */
    seq_t fact = (seq_t)1;
    for (idx_t i = 2; i <= r; ++i)
        fact *= i;

    return fact * sum;
}
