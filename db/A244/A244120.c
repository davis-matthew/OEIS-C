/*
 * A244120  Triangle read by rows: coefficients T(n,k) of a binomial
 *          decomposition of n^n as Sum(k=0..n)T(n,k)*binomial(n,k).
 * https://oeis.org/A244120
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,c) where T(n,k) = n*(n-k)^(k-1)*k^(n-k) for k>0;
 *            T(n,0)=0^n with 0^0 = 1. (r,c) are the row and column of the
 *            n‑th triangular element, rows counted from 0 and row r having
 *            r+1 entries
 * Range:     u8 n <= 22, u16 n <= 39, u32 n <= 83, u64 n <= 217,
 *            u128 n <= 542, big any n
 * Helpers:   pow_int, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_mul.h"
#include "../utils/pow_int.h"

seq_t a(idx_t n)
{
    /* Handle the special case n == 0: T(0,0) = 1 (0^0 = 1). */
    if (n == 0)
        return (seq_t)1;

    /* Find row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2. */
    idx_t low = 0, high = n, r = 0;
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 off = (unsigned __int128)mid * (mid + 1) / 2;
        if (off <= n) {
            low = mid + 1;
            r = mid;
        } else {
            high = mid - 1;
        }
    }

    /* Offset of the first element in row r. */
    idx_t offset = (idx_t)((unsigned __int128)r * (r + 1) / 2);
    idx_t c = n - offset;   /* column index within row r (0‑based) */

    /* For column 0 return the convention: 1 if r==0 else 0. */
    if (c == 0)
        return (r == 0 ? (seq_t)1 : (seq_t)0);

    /* Compute T(r,c) for c > 0:  T(r,c) = r * (r-c)^(c-1) * c^(r-c). */
    seq_t row_val = (seq_t)r;

    seq_t base1 = (seq_t)(r - c);
    idx_t  exp1 = c - 1;

    seq_t base2 = (seq_t)c;
    idx_t  exp2 = r - c;

    seq_t pow1  = pow_int(base1, exp1);   /* (r-c)^(c-1) */
    seq_t pow2  = pow_int(base2, exp2);   /*  c   ^(r-c)  */
    seq_t prod  = seq_mul(pow1, pow2);
    seq_t result = seq_mul(row_val, prod);

    return result;
}
