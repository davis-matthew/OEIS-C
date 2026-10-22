/*
 * A393312  Triangle read by rows: T(n,k) = (n-k-1)! * binomial(n,k) if n+k is
 *          odd, else 0.
 * https://oeis.org/A393312
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 55, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   binomial, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle read by rows:  
   T(n,k) = (n-k-1)! * binomial(n,k)  if (n+k) is odd,  
            0                          otherwise.  
   a(n) is the concatenation of the rows of T.  */
#include "../utils/factorial.h"
#include "../utils/binomial.h"

/* Find the row r such that r*(r+1)/2 <= idx < (r+1)*(r+2)/2 */
static inline idx_t _row_for_idx(idx_t idx)
{
    idx_t low = 0, high = 1;
    while ((unsigned __int128)high * (high + 1) / 2 <= idx)
        high <<= 1;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if ((unsigned __int128)mid * (mid + 1) / 2 <= idx)
            low = mid + 1;
        else
            high = mid;
    }
    return low - 1;
}

seq_t a(idx_t n)
{
    idx_t r = _row_for_idx(n);
    idx_t start = r * (r + 1) / 2;
    idx_t k = n - start;

    /* If n + k is even, the triangle entry is zero. */
    if (((r + k) & 1) == 0)
        return (seq_t)0;

    idx_t fact_exp = r - k - 1;   /* r + k odd guarantees fact_exp >= 0 */
    seq_t f = factorial(fact_exp);
    seq_t b = binomial(r, k);
    return f * b;
}
