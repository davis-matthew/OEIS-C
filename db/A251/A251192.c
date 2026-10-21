/*
 * A251192  T(n,k)=Number of (n+2)X(k+2) 0..1 arrays with nondecreasing sum of
 *          every three consecutive values in every row and column.
 * https://oeis.org/A251192
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 11, u32 n <= 59, u64 n <= 262, u128 n <= 794,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A251192 – Number of (n+2)×(k+2) binary arrays with
   the property that the sum of every three consecutive entries
   in each row and in each column is non‑decreasing.
   The sequence a(n) lists the table values by antidiagonal order,
   i.e. (1,1),(1,2),(2,1),(1,3),(2,2),(3,1),… .

   Formula (derived from the combinatorial decomposition):
   Let (N,K) be the pair (row, column) corresponding to index n
   as described above.
   Define for each residue class p∈{0,1,2}

       r[p] = ⌊(N+1−p)/3⌋   = number of rows with row≡p (mod 3),
       c[q] = ⌊(K+1−q)/3⌋   = number of columns with col≡q (mod 3).

   Then

         a(n) = ∏_{p=0}^{2} ∏_{q=0}^{2}  C(r[p] + c[q], r[p])

   where C(a,b) is the binomial coefficient (choosing a subset of size b from a).
   The product runs over the 9 residue sub‑matrices; each sub‑matrix is a
   monotone binary matrix of size r[p]×c[q] and its number of possibilities
   equals C(r[p]+c[q], r[p]).
 */

#include <math.h>
#include "../utils/binomial.h"   /* provides seq_t binomial(idx_t a, idx_t b) */

seq_t a(idx_t n)
{
    /* --- 1. Recover (N,K) from index n by antidiagonal enumeration --- */
    /* n is 1‑based.  Find smallest s with (s‑1)s/2 ≥ n.                     */
    double disc = sqrt((double)(1ULL + 8ULL * n));
    idx_t s = (idx_t)ceil((1.0 + disc) / 2.0);

    /* adjust for possible rounding error */
    while ((s - 1) * s / 2 < n) ++s;
    while ((s - 2) * (s - 1) / 2 >= n) --s;

    idx_t prev = (s - 2) * (s - 1) / 2;
    idx_t offset = n - prev - 1;          /* zero‑based offset on diagonal     */
    idx_t N = offset + 1;                 /* row index (1‑based)               */
    idx_t K = s - N;                      /* column index (1‑based)            */

    /* --- 2. Count rows and columns in each residue class 0,1,2 --- */
    idx_t H = N + 2;   /* actual number of rows */
    idx_t W = K + 2;   /* actual number of columns */

    idx_t r[3] = {0,0,0};
    idx_t c[3] = {0,0,0};

    for (int p = 0; p < 3; ++p) {
        if (p < H) {
            r[p] = (H - 1 - p) / 3 + 1;
        } else {
            r[p] = 0;
        }
    }

    for (int q = 0; q < 3; ++q) {
        if (q < W) {
            c[q] = (W - 1 - q) / 3 + 1;
        } else {
            c[q] = 0;
        }
    }

    /* --- 3. Compute product of binomials for all 9 sub‑matrices --- */
    seq_t result = 1;
    for (int p = 0; p < 3; ++p) {
        for (int q = 0; q < 3; ++q) {
            idx_t a = r[p] + c[q];
            idx_t b = r[p];
            seq_t factor = binomial(a, b);
            result *= factor;
        }
    }

    return result;
}
