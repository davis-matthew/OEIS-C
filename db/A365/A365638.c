/*
 * A365638  Triangular array read by rows: T(n, k) is the number of ways that
 *          a k-element transitive tournament can occur as a subtournament of
 *          a larger tournament on n labeled vertices.
 * https://oeis.org/A365638
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = T(row,k) where row = floor((√(8n+1) – 1)/2), k = n –
 *            row(row+1)/2 and T(n,k) = binomial(n,k) * k! * 2^(C(n,2) –
 *            C(k,2))
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 37, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   binomial, factorial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/factorial.h"
#include "../utils/pow2.h"

/* Find the row of the triangle that contains the index n */
static idx_t row_from_index(idx_t idx)
{
    idx_t lo = 0, hi = idx;
    while (lo < hi) {
        /* mid is biased upward to ensure no infinite loop */
        idx_t mid = lo + (hi - lo + 1) / 2;
        unsigned __int128 pref = (unsigned __int128)mid * (mid + 1) / 2;
        if (pref <= idx)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    /* Interpret the sequence index into (row, column) of the
       triangular array.  The array has row 0, column 0; row 1 has
       columns 0,1; etc.  The number of terms preceding row r is
       r(r+1)/2. */
    idx_t row = row_from_index(n);
    unsigned __int128 pref128 = (unsigned __int128)row * (row + 1) / 2;
    idx_t prefix = (idx_t)pref128;          /* prefix ≤ n by construction */
    idx_t k = n - prefix;                   /* column within the row */

    /* Compute T(row,k) = C(row,k) · k! · 2^(C(row,2) – C(k,2)) */
    seq_t value = binomial(row, k);         /* C(row,k) */
    value = value * factorial(k);           /* · k! */

    /* exponent = C(row,2) – C(k,2) */
    unsigned __int128 exp128 = ((unsigned __int128)row * (row - 1)) / 2
                             - ((unsigned __int128)k * (k - 1)) / 2;
    idx_t exp = (idx_t)exp128;              /* fits in idx_t for all intended n */

    value = value * pow2(exp);              /* · 2^exponent */
    return value;
}
