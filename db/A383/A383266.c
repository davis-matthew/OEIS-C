/*
 * A383266  Triangle read by rows: For n, k >= 2 T(n, k) is defined as the
 *          exponent of the highest power e of k such that k^e <= n. Otherwise
 *          T(n, 0) = n^2 and T(n, 1) = n.
 * https://oeis.org/A383266
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle read by rows.  For each non‑negative integer n we first
   determine the row r and the column c that contain the nth entry
   when the triangle is read line‑by‑line (row 0, then row 1,
   etc.  The triangle entry T(r,c) is:
        T(r,0) = r*r
        T(r,1) = r
        T(r,c) = largest e with c^e ≤ r   (c ≥ 2)
   The function returns T(r,c) in the type seq_t.  The indices and
   arithmetic are performed in idx_t (unsigned 64‑bit) to avoid
   overflow for n up to about 10 000.  */
/* NO additional libraries except <stdbool.h>, which is allowed for
   the bool type used in helper prototypes. */

#include <stdbool.h>

/* Integer square‑root floor for 64‑bit values
   (no floating point, no <math.h>). */
static inline idx_t floor_sqrt_u64(idx_t n)
{
    idx_t low = 0, high = n;
    while (low < high) {
        /* mid = ⌈(low+high)/2⌉  */
        idx_t mid = low + ((high - low + 1) >> 1);
        if (mid <= n / mid)          /* mid*mid ≤ n  */
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/* Return the n‑th element of the flattened triangle. */
seq_t a(idx_t n)
{
    /* Find the row r such that  r(r+1)/2 ≤ n < (r+1)(r+2)/2  */
    idx_t v    = n * 8 + 1;                 /* 8·n + 1   */
    idx_t s    = floor_sqrt_u64(v);
    idx_t r    = (s - 1) / 2;
    idx_t base = r * (r + 1) / 2;           /* index of first entry of row r */
    idx_t c    = n - base;                  /* column within the row (0‑based) */

    /* Compute T(r,c) */
    if (c == 0) {
        seq_t rr = (seq_t)r;
        return rr * rr;                     /* r^2 */
    } else if (c == 1) {
        return (seq_t)r;                    /* r   */
    } else {
        /* Find largest e with c^e ≤ r  by repeated multiplication. */
        idx_t pow = 1;
        seq_t  exp = 0;
        while (pow <= r / c) {              /* pow·c ≤ r  */
            pow *= c;
            exp++;
        }
        return exp;
    }
}
