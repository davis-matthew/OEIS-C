/*
 * A244141  Triangle read by rows: terms T(n,k) of a binomial decomposition of
 *          n*(-1)^n as Sum(k=0..n)T(n,k).
 * https://oeis.org/A244141
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = T(r,k) where r = floor((sqrt(8n+1)-1)/2), k = n -
 *            r*(r+1)/2, and T(r,k) = 0 if k == 0, = -1 if k == 1 and r == 1,
 *            = 0 if k == 1 and r > 1, = (-1)^k * k * (k-2)^(r-2) *
 *            binomial(r,k) if k > 1
 * Range:     i8 n <= 18, i16 n <= 33, i32 n <= 72, i64 n <= 163,
 *            i128 n <= 396, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

static seq_t ipow(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp > 0) {
        if (exp & 1)
            result *= base;
        exp >>= 1;
        if (exp)
            base *= base;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* find row r : r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t r = 0;
    while ((r * (r + 1)) / 2 <= n)
        ++r;
    --r;
    idx_t start = r * (r + 1) / 2;
    idx_t k = n - start;     /* column in row r */

    if (k == 0)
        return 0;
    if (k == 1) {
        if (r == 1)
            return (seq_t)-1;
        return 0;
    }

    /* k > 1 */
    seq_t res = binomial(r, k);
    res *= (seq_t)k;
    seq_t pow_val = ipow((seq_t)(k - 2), (idx_t)(r - 2));
    res *= pow_val;
    if (k & 1)
        res = -res;
    return res;
}
