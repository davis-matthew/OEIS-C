/*
 * A384133  Triangle read by rows: T(n,k) is the number of linear intervals of
 *          height k in the Tamari lattice Tam_n (0 <= k < n).
 * https://oeis.org/A384133
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 37, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   _entries_before, binom, catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A384133 (triangle flattened row-wise)
   Formula: a(n) = T(r,k) where r is the smallest integer with r(r+1)/2 ≥ n
   and k = n - (r-1)r/2 - 1 (0‑based column).
   T(r,0) = binomial(2r,r)/(r+1) = catalan(r)
   T(r,1) = binomial(2r-1,r-2)
   T(r,k) = 2 * binomial(2r-k,r-k-1) for k>1                           */

#include <assert.h>
#include <stdbool.h>
#include "../utils/binom.h"
#include "../utils/catalan.h"
#include "../utils/_entries_before.h"

/* smallest r with r(r+1)/2 >= n (n >= 1) */
static inline idx_t _row_of(idx_t n)
{
    idx_t r = 1;
    idx_t sum = 1;
    while (sum < n) {
        ++r;
        sum += r;
    }
    return r;
}

seq_t a(idx_t n)
{
    assert(n >= 1);

    const idx_t r = _row_of(n);
    const idx_t prev = _entries_before(r);
    const idx_t k = n - prev - 1;

    if (k == 0) {
        return catalan(r);
    } else if (k == 1) {
        return binom(2 * r - 1, r - 2);
    } else {
        seq_t val = binom(2 * r - k, r - k - 1);
        return (seq_t)2 * val;
    }
}
