/*
 * A317028  Triangle read by rows: T(0,0) = 1; T(n,k) = 8 * T(n-1,k) +
 *          T(n-2,k-1) for k = 0..floor(n/2); T(n,k)=0 for n or k < 0.
 * https://oeis.org/A317028
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabf, nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 35, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   _pow8
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle T(n,k) is defined by  
   T(0,0) = 1,  
   T(n,k) = 8*T(n‑1,k) + T(n‑2,k‑1)   (0 ≤ k ≤ ⌊n/2⌋),  
   T(n,k) = 0 for n<0 or k<0.  
   The sequence a(n) lists the triangle row‑wise, i.e. a(0)=T(0,0),
   a(1)=T(1,0), a(2)=T(2,0), a(3)=T(2,1), … .
   A closed form for each entry is  
       T(n,k) = C(n‑k, k) · 8ⁿ⁻²ᵏ   (choose with binomial coefficient). */

#include <stdint.h>
#include <stddef.h>
#include "../utils/_pow8.h"

/* Helper: binomial coefficient C(n,k) for 0≤k≤n, using 128‑bit
   intermediate to avoid overflow when the result fits in seq_t. */
static seq_t
_binom(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;          /* use symmetry */
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        __int128 tmp = (__int128)res * (n - k + i);
        res = (seq_t)(tmp / i);          /* division is exact */
    }
    return res;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    idx_t row = 0, cumulative = 0;

    /* Find the row containing the n‑th term (row indexing from 0). */
    for (;; ++row) {
        idx_t len = row / 2 + 1;          /* number of entries in this row */
        if (cumulative + len > n) break; /* n lies in this row */
        cumulative += len;
    }

    idx_t k = n - cumulative;              /* column index in the row */

    /* Compute T(row,k) = C(row‑k, k) * 8^(row‑2k) */
    seq_t binom = _binom(row - k, k);
    seq_t pow8  = _pow8(row - 2 * k);
    return binom * pow8;
}
