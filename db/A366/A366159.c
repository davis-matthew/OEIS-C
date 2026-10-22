/*
 * A366159  Triangle read by rows: T(n, k) = Sum_{i=0..k-2} (-1)^(i+2) *
 *          (k-i-1)^n * binomial(k,i).
 * https://oeis.org/A366159
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,k) = Sum_{i=0..k-2} (-1)^(i+2) * (k-i-1)^n * binomial(k,i)
 *            a(n) = T(row,column) read by rows with offset 2 a(2)=T(2,2),
 *            a(3)=T(3,2), a(4)=T(3,3), a(5)=T(4,2), …
 * Range:     u8 n <= 13, u16 n <= 26, u32 n <= 72, u64 n <= 182,
 *            u128 n <= 484, big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n < 2)   /* undefined below the sequence offset */
        return 0;

    /* position in the sequence relative to the offset 2 (zero‑based) */
    idx_t m = n - 2;

    /* find the row r such that cumulative terms up to row r-1
       is <= m < cumulative terms up to row r                     */
    idx_t r = 2;
    while ((r - 1) * r / 2 <= m)
        ++r;

    /* cumulative terms up to the previous row (row r-1) */
    idx_t prev = (r - 2) * (r - 1) / 2;
    /* position inside the current row (1‑based) */
    idx_t p = m - prev + 1;

    /* corresponding triangle indices: n' = r, k' = p + 1          */
    idx_t nprime = r;
    idx_t kprime = p + 1;

    seq_t sum = 0;
    for (idx_t i = 0; i <= kprime - 2; ++i)
    {
        /* (k-i-1)^nprime */
        seq_t base = (seq_t)(kprime - i - 1);
        seq_t pw   = big_pow(base, (seq_t)nprime);

        /* binomial coefficient C(kprime, i) */
        seq_t comb = binomial(kprime, i);

        /* term with sign */
        seq_t term = pw * comb;
        if (((i + 2) & 1) == 0)
            sum += term;
        else
            sum -= term;
    }

    return sum;
}
