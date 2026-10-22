/*
 * A305838  Triangle read by rows: T(0,0)= 1; T(n,k)= T(n-1,k) + 5*T(n-2,k-1)
 *          for k = 0..floor(n/2); T(n,k)=0 for n or k < 0.
 * https://oeis.org/A305838
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabf, nonn, easy
 * Formula:   a(n) = T(row, k) = C(row - k, k) * 5^k where row is the smallest
 *            integer with S(row) > n (S(r) = #{entries up to row r}, S(2m) =
 *            (m+1)^2, S(2m+1) = (m+1)(m+2)), and k = n - S(row-1). This
 *            follows from T(n,k) = binom(n-k,k)*5^k, 0≤k≤⌊n/2⌋. The
 *            implementation uses only integer arithmetic and a simp...
 * Range:     u8 n <= 18, u16 n <= 46, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute the cumulative number of triangle entries up to and including
   row r.  For even r = 2m: S = (m+1)^2.
   For odd r = 2m+1:  S = (m+1)*(m+2).                                       */
static inline idx_t cumulative(idx_t r)
{
    if ((r & 1ULL) == 0)          /* even */
    {
        idx_t m = r >> 1;         /* m = r/2 */
        idx_t v = m + 1;
        return v * v;
    }
    else                          /* odd */
    {
        idx_t m = (r - 1) >> 1;   /* m = (r-1)/2 */
        return (m + 1) * (m + 2);
    }
}

/* Compute binomial coefficient C(n, k) using multiplicative formula.
   n >= k, k <= n.  The loop is performed only up to k to keep
   intermediate values as small as possible.                                */
static seq_t binom(idx_t n, idx_t k)
{
    if (k == 0 || k == n)
        return (seq_t)1;
    if (k > n / 2)                /* use symmetry */
        k = n - k;
    seq_t res = (seq_t)1;
    for (idx_t i = 1; i <= k; ++i)
    {
        res = res * (seq_t)(n - k + i) / (seq_t)i;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* Locate the row in which a(n) lies.  Start from row 0 and
       increment until the cumulative count exceeds n.             */
    idx_t row = 0;
    while (cumulative(row) <= n)
        ++row;

    /* The desired element is the (n - cumulative(row-1))–th entry
       in this row (0‑based).                                         */
    idx_t prev_cum = (row == 0) ? 0 : cumulative(row - 1);
    idx_t k = n - prev_cum;                     /* column index */

    /* Quick answer for the trivial cases.                           */
    if (k == 0)
        return (seq_t)1;

    /* T(row, k) = binom(row - k, k) * 5^k                                  */
    seq_t comb = binom(row - k, k);

    seq_t pow5 = (seq_t)1;
    for (idx_t i = 0; i < k; ++i)
        pow5 *= (seq_t)5;

    return comb * pow5;
}
