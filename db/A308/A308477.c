/*
 * A308477  Square array A(n,k), n >= 1, k >= 0, read by antidiagonals: A(n,k)
 *          = Sum_{j=1..n, gcd(n,j) = 1} j^k.
 * https://oeis.org/A308477
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = Sum_{j=1..k, gcd(k,j)=1} j^m where the pair (k,m) is
 *            obtained by enumerating the square array A(n,k) =
 *            Σ_{j=1..n,gcd(n,j)=1} j^k along antidiagonals. For index n, let
 *            d be the diagonal number such that n lies in the d-th diagonal
 *            (with d≥1). Then k = d - 1 + 1? See implementation for mappi...
 * Range:     u8 n <= 40, u16 n <= 82, u32 n <= 195, u64 n <= 472,
 *            u128 n <= 1188, big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

static inline int mygcd(int a, int b)
{
    if (a < 0) a = -a;
    if (b < 0) b = -b;
    while (b != 0) {
        int t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* Find the diagonal d such that n is in the d-th antidiagonal.
       The number of terms up to diagonal d is T(d)=d*(d+1)/2. */
    idx_t d = 1;
    while (d * (d + 1) / 2 < n) ++d;

    /* Number of terms before this diagonal. */
    idx_t prev = d * (d - 1) / 2;
    idx_t pos   = n - prev - 1;          /* zero‑based position on diagonal d */

    idx_t k = 1 + pos;                 /* row number (n in A(n,k))   */
    idx_t m = d - k;                   /* column number (k in A(n,k)) */

    /* Compute the sum Σ_{j=1..k, gcd(k,j)=1} j^m */
    seq_t sum = 0;
    for (idx_t j = 1; j <= k; ++j)
    {
        if (mygcd((int)k, (int)j) == 1)
        {
            seq_t term = big_pow((seq_t)j, (seq_t)m);
            sum += term;
        }
    }
    return sum;
}
