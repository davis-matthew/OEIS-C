/*
 * A390879  Triangle read by rows: T(n,k) = A237271(k)*(n-k+1), 1 <= k <= n.
 * https://oeis.org/A390879
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where r = ceil((sqrt(8*n+1)-1)/2), k = n -
 *            (r-1)*r/2, T(r,k) = A237271(k)*(r-k+1) with A237271(k) defined
 *            as: if k even and divisible by 3: return 1 else: return
 *            (Ω(odd_part(k)) + 1) where Ω(x) is the total number of prime
 *            factors of x (counted with multiplicity), and odd_part(k) is...
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   A237271_val
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>
#include "../utils/A237271_val.h"

seq_t a(idx_t n)
{
    /* Compute row r such that r*(r+1)/2 >= n */
    double d = sqrt((long double)(8ULL * n + 1ULL));
    idx_t r = (idx_t)((d - 1.0L) / 2.0L);
    /* Rounding may fall one short */
    if (r * (r + 1ULL) / 2ULL < n)
        ++r;

    /* Compute column k and factor (r-k+1) */
    idx_t pre = (r - 1ULL) * r / 2ULL;   /* sum of first (r-1) rows */
    idx_t k   = n - pre;                 /* 1‑based column index */
    idx_t factor = r - k + 1ULL;

    seq_t val = A237271_val(k);
    return (seq_t)factor * (seq_t)val;
}
