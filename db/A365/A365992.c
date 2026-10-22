/*
 * A365992  Square array read by ascending antidiagonals: T(n,k) is the parity
 *          of the k-th iterate of the 5x+1 function started at n, with n >= 1
 *          and k >= 0.
 * https://oeis.org/A365992
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 3627, u16 n <= 3052, u32 n <= 8887, u64 n <= 8887,
 *            u128 n <= 8887, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute OEIS A365992: parity of the k‑th iterate of the 5x+1 function
 * starting at n, where (n,k) is obtained by reading the square array
 * by ascending antidiagonals (n >= 1, k >= 0).  For a given index i (>=1),
 * we determine the corresponding (n,k), iterate the 5x+1 map k times
 * on n, and return 0 or 1 (the parity of the final iterate). */
seq_t a(idx_t i)
{
    /* Find the antidiagonal d such that i falls on it:
       number of elements before diagonal d is d*(d+1)/2,
       number of elements up to and including diagonal d is
       (d+1)*(d+2)/2. */
    idx_t d = 0;
    while ((d + 1) * (d + 2) / 2 < i)
        ++d;

    /* Position inside this diagonal (1‑based). */
    idx_t prev = d * (d + 1) / 2;
    idx_t pos  = i - prev;

    /* Convert diagonal position to (n,k):
       n = d + 2 - pos  (row index, >= 1)
       k = pos - 1      (column index, >= 0)           */
    idx_t n = d + 2 - pos;
    idx_t k = pos - 1;

    seq_t x = (seq_t)n;                     /* current value of the iterate */

    /* Iterate k times. */
    for (idx_t step = 0; step < k; ++step)
    {
        if ((x & 1) == 0)                  /* even:  T5(x) = x/2     */
            x = x / 2;
        else                               /* odd :  T5(x) = (5x+1)/2*/
            x = (x * 5 + 1) / 2;
    }

    /* Return the parity of the final iterate. */
    return (x & 1) ? (seq_t)1 : (seq_t)0;
}
