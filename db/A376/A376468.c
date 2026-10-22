/*
 * A376468  Triangle T read by rows: T(n, k) = (n^2 - 2*n + 3 - (-1)^n + n^2
 *          mod 8) / 2 + 4*k.
 * https://oeis.org/A376468
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 227, u16 n <= 9575, u32 n <= 9575, u64 n <= 9575,
 *            u128 n <= 9575, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A376468 – permutation of the naturals read row‑by‑row.
   Triangle entry:  T(n,k) = ((n*n - 2*n + 3 - (-1)^n + (n*n mod 8)) / 2) + 4*k.
   For a global index N, let r = ⌊(√(8 N+1)−1)/2⌋ be the row and
   c = N − r(r+1)/2 the column.  Then a(N) = T(r,c). */
#include <math.h>

seq_t a(idx_t n)
{
    /* find row r by a floating‑point approximation and correct it */
    double d = sqrt((double)n * 8.0 + 1.0);
    idx_t r = (idx_t)((d - 1.0) / 2.0);

    /* correct possible off‑by‑one errors due to rounding */
    __int128 sum_prev = (__int128)r * (r + 1) / 2;   /* Σ_{i=0}^{r-1} (i+1) */
    __int128 sum_next = (__int128)(r + 1) * (r + 2) / 2; /* Σ_{i=0}^{r} (i+1) */

    while (sum_prev > n) {
        r--;
        sum_prev = (__int128)r * (r + 1) / 2;
        sum_next = (__int128)(r + 1) * (r + 2) / 2;
    }
    while (sum_next <= n) {
        r++;
        sum_prev = (__int128)r * (r + 1) / 2;
        sum_next = (__int128)(r + 1) * (r + 2) / 2;
    }

    idx_t column = n - (idx_t)sum_prev;          /* k */

    /* compute base value T(r,0) */
    seq_t rr = (seq_t)r * (seq_t)r;              /* r²  */
    seq_t term = rr - (seq_t)2 * (seq_t)r + (seq_t)3;   /* r²−2r+3  */

    if ((r & 1) == 0)                 /* even row: subtract 1  */
        term -= (seq_t)1;
    else                              /* odd row:  add 1      */
        term += (seq_t)1;

    term += (rr & (seq_t)7);          /* add r² mod 8          */
    seq_t base = term / (seq_t)2;     /* divide by 2           */

    seq_t result = base + (seq_t)4 * (seq_t)column; /* T(r, k)   */
    return result;
}
