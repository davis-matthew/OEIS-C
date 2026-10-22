/*
 * A398477  Triangle read by rows: T(n, k + 1) = T(n, k) * (n + k) / (n * (k +
 *          1)) for k <= n, starting at T(n, 0) = n^n, (row recurrence).
 * https://oeis.org/A398477
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 27, u32 n <= 54, u64 n <= 135, u128 n <= 377,
 *            big any n
 * Helpers:   binom, catalan, seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A398477: Triangle read by rows
   a(n) is the value T( r , k ) where
   r = floor((sqrt(8*n+1)-1)/2)   (row index)
   k = n - r*(r+1)/2              (column index)
   T(n,k) = binomial(n+k , k+1) * n^(n-k-1)   (for k < n)
   T(n,n) = Catalan(n) */
 
#include <stdint.h>
#include "../utils/binom.h"    /* binom(idx_t a, idx_t b)  */
#include "../utils/catalan.h"  /* catalan(idx_t k)         */
#include "../utils/seq_mul.h"  /* seq_mul(seq_t a, seq_t b) */
#include "../utils/seq_pow.h"  /* seq_pow(seq_t base, idx_t exp) */
 
/* find row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
static inline idx_t row_from_index(idx_t n) {
    idx_t low = 0, high = n;
    while (low < high) {
        idx_t mid = low + (high - low + 1) / 2;
        idx_t s = mid * (mid + 1) / 2;
        if (s <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}
 
seq_t a(idx_t n) {
    /* Determine row r and column k */
    idx_t r = row_from_index(n);
    idx_t k = n - r * (r + 1) / 2;
 
    if (r == 0)   /* base row */
        return (seq_t)1;
 
    if (k == r)   /* diagonal entry is a Catalan number */
        return catalan(r);
 
    /* For k < r: use T(n,k) = binomial(n+k , k+1) * n^(n-k-1) */
    seq_t pow_val   = seq_pow((seq_t)r, r - k - 1);
    seq_t binom_val = binom(r + k, k + 1);
    return seq_mul(binom_val, pow_val);
}
