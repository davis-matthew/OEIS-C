/*
 * A281293  Triangular array of generalized Narayana Numbers T(n,k) =
 *          3*binomial(n+1,k)* binomial(n-3,k-1)/(n+1) for n >= 2 and 0 <= k
 *          <= n-2, read by rows.
 * https://oeis.org/A281293
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = value of the n-th term of the flattened * triangular
 *            array of generalized Narayana numbers * T(m,k) =
 *            3*binomial(m+1,k)*binomial(m-3,k-1)/(m+1), * with row m starting
 *            at n = 2 and columns 0..m-2. * The sequence is read by rows,
 *            offset 2 (a(2) is first term)
 * Range:     u8 n <= 14, u16 n <= 50, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   binomial, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/binomial.h"
#include "../utils/gcd.h"

/* Construct the n-th term (n >= 2).  For indices < 2 the
 * behaviour is undefined; we return 0. */
seq_t a(idx_t n)
{
    if (n < 2) return 0;

    /* 1‑based index of the term in the flattened array */
    idx_t j = n - 1;

    /* Find the row r such that S(r‑1) < j <= S(r),
     * where S(r) = r*(r‑1)/2 is the cumulative number up
     * to and including row r (rows are numbered from 2). */
    idx_t low  = 2, high = 2;
    /* Double high until it is large enough */
    while (((unsigned __int128)high * (high - 1) / 2) < (unsigned __int128)j)
        high <<= 1;

    /* Binary search for the correct row */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 tot = (unsigned __int128)mid * (mid - 1) / 2;
        if (tot >= (unsigned __int128)j)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;                    /* row number */

    /* Position within the row (1‑based) */
    unsigned __int128 prev = (unsigned __int128)(r - 1) * (r - 2) / 2;
    idx_t pos = j - (idx_t)prev;       /* 1 .. r‑1 */
    idx_t k   = pos - 1;               /* 0 .. r‑2 */

    /* Special handling for the very first row.
     * Row 2 has a single entry equal to 1.  All other rows start with 0. */
    if (r == 2)
        return (seq_t)1;
    if (k == 0)
        return (seq_t)0;

    /* Compute T(r,k) = 3*binom(r+1,k)*binom(r-3,k-1)/(r+1) */
    seq_t a1 = binomial(r + 1, k);
    seq_t a2 = binomial(r - 3, k - 1);
    seq_t val = a1 * a2;
    val = val * (seq_t)3;
    val = val / (seq_t)(r + 1);
    return val;
}
