/*
 * A365991  Square array read by ascending antidiagonals: T(n,k) is the k-th
 *          iterate of the 5x+1 function started at n, with n >= 1 and k >= 0.
 * https://oeis.org/A365991
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 111, u16 n <= 456, u32 n <= 5029, u64 n <= 9252,
 *            u128 n <= 9252, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Function computing OEIS A365991: “square array read by ascending antidiagonals”
   of the 5x+1 function.  For an index n (1‑based), find the diagonal s with
   s*(s+1)/2 ≥ n, compute the offset within that diagonal, determine the
   starting row, then iterate the 5x+1 function k times. */

seq_t a(idx_t n)
{
    /* If the caller supplies n=0, we simply return 0. */
    if (n == 0) return (seq_t)0;

    /* Find the smallest s such that s*(s+1)/2 ≥ n.
       Since s grows roughly as √(2n), a simple loop is fast enough. */
    idx_t s = 0;
    while (s * (s + 1) / 2 < n) s++;

    /* Number of elements before diagonal s. */
    idx_t prev = (s - 1) * s / 2;

    /* Offset (0‑based) of the desired element on diagonal s. */
    idx_t offset = n - prev - 1;      /* 0 ≤ offset < s */

    /* On diagonal s we enumerate rows n decreasing from s to 1.
       Therefore:
           row   = s - offset  (starting value)
           k     = offset      (number of iterations) */
    idx_t row = s - offset;
    idx_t k   = offset;

    /* Start at value = row and iterate k times. */
    seq_t val = (seq_t)row;
    while (k-- > 0) {
        if ((val & 1) != 0)          /* odd: (5x+1)/2 */
            val = (5 * val + 1) / 2;
        else                         /* even: x/2   */
            val = val / 2;
    }
    return val;
}
