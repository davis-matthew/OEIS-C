/*
 * A327580  Triangle read by rows: T(n,k) = Sum_{1<=j*k<=n} cos(Pi*(j*k-1)/2).
 * https://oeis.org/A327580
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl
 * Formula:   not stated
 * Range:     i8 n <= 4950, i16 n <= 4950, i32 n <= 4950, i64 n <= 4950,
 *            i128 n <= 4950, big any n
 * Helpers:   triangular_count
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * The sequence a(n) is obtained by flattening the triangle T(n,k) row‑by‑row
 * where
 *     T(n,k) = sum_{1<=j*k<=n} cos(pi*(j*k-1)/2)
 * and n,k >= 1.
 *
 * For odd k, T(n,k) reduces to
 *     ((floor(n/k)+1)/2  mod 2) * (-1)^{(k-1)/2}
 * (for even k, T(n,k)=0).  The flattened triangle yields exactly the
 * sequence listed in the OEIS entry.
 *
 * The function a(idx_t n) therefore finds the row r and column c in the
 * triangle that correspond to the n‑th flattened element and returns
 * T(r,c).  All operations are integer arithmetic; no floating‑point
 * arithmetic is used for the final value.
 */

#include <stdint.h>
#include "../utils/triangular_count.h"

/* Determine the row r such that triangular_count(r-1) < n <= triangular_count(r) */
static inline idx_t row_of(idx_t n)
{
    /* Binary search on [1, n] – row cannot exceed n */
    idx_t low = 1, high = n;
    while (low < high)
    {
        idx_t mid = low + (high - low) / 2;
        if (triangular_count(mid) >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

/* Compute the triangle entry T(n,k) for 1 <= k <= n */
static inline seq_t triangle_entry(idx_t n, idx_t k)
{
    if ((k & 1) == 0)               /* even k => zero */
        return 0;

    idx_t q = n / k;                /* floor(n/k)   */
    idx_t parity = ((q + 1) / 2) & 1; /* (ceil(q/2)) & 1 */

    if (!parity)                    /* no contribution */
        return 0;

    /* sign = (-1)^{(k-1)/2}  */
    seq_t sign = ((k & 3) == 1) ? 1 : -1;
    return sign;                    /* parity is 1, so multiply by it */
}

seq_t a(idx_t n)
{
    if (n == 0)                       /* n is 1‑based per offset */
        return 0;

    idx_t r = row_of(n);               /* row containing the n‑th element */
    idx_t prev = triangular_count(r - 1);
    idx_t c = n - prev;                /* column within the row */

    return triangle_entry(r, c);
}
