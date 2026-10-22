/*
 * A343155  Irregular triangle T read by rows: T(n, k) is the sum of the
 *          consecutive integers placed along the k-th turn of the spiral of
 *          the n X n matrix defined in A126224.
 * https://oeis.org/A343155
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   _T_value
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A343155 – Irregular triangle T read by rows
 *
 *  The triangle T(n,k) is given by
 *
 *      T(n,k) = 2*(2*k - n - 1)*(3 + 8*k*(k - n - 1) + 4*n)
 *               + n^2   if n is odd and k = (n+1)/2
 *               + 0     otherwise
 *
 *  The sequence a(n) is the triangle flattened row‑by‑row.
 *  The routine below converts a global index n to the corresponding
 *  (row,column) pair and evaluates T(n,column) using the closed form
 *  above.
 *
 *  All arithmetic on seq_t values is performed using the normal
 *  integer operators, so the implementation works for arbitrary‑precision
 *  seq_t as well as for ordinary 8/16/32/64‑bit unsigned types.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/_T_value.h"

/* ---------- Integer square root for unsigned long long ---------- */
static inline idx_t _floor_isqrt_u64(idx_t v)
{
    if (v < 2) return v;
    idx_t x = v;
    idx_t y = (x + 1) >> 1;
    while (y < x) {
        x = y;
        y = (x + v / x) >> 1;
    }
    return x;
}

/* ---------- Public routine a(n) --------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;       /* undefined for n<1, but harmless */

    /* 1. Find the row r (1‑based).  The cumulative size after row r is
       (t^2)   for odd r = 2t-1
       (t^2+t) for even r = 2t */
    idx_t t, r;
    /* Compute t = floor((sqrt(4*n-3)+1)/2) using integer sqrt */
    idx_t sqrt_val = _floor_isqrt_u64(4 * n - 3);
    t = (sqrt_val + 1) >> 1;

    if (n <= t * t) {
        r = (t << 1) - 1;      /* odd row 2t-1 */
    } else {
        r = t << 1;            /* even row 2t    */
    }

    /* 2. Determine the column k (1‑based) within the row */
    /* Number of entries in row r is ceil(r/2) = t */
    idx_t k = n - ( (r & 1U) ? (t - 1) * t : t * t );  /* prev row count */

    /* 3. Evaluate T(r,k) */
    return _T_value(r, k);
}
