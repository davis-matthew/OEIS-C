/*
 * A322699  Square array A(n,k), n >= 0, k >= 0, read by antidiagonals, where
 *          A(n,k) is 1/2 * (-1 + Sum_{j=0..k}
 *          binomial(2*k,2*j)*(n+1)^(k-j)*n^j).
 * https://oeis.org/A322699
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 37, u32 n <= 80, u64 n <= 213,
 *            u128 n <= 600, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A322699: a(n) = A(i,j) on the antidiagonals.
   Recurrence:
        A(i,0) = 0,
        A(i,1) = i,
        A(i,k) = (4*i+2)*A(i,k-1) - A(i,k-2) + 2*i   for k > 1
   Antidiagonal mapping:
        Find diagonal d such that d*(d+1)/2 ≤ n < (d+1)*(d+2)/2.
        sum = d*(d+1)/2   (elements before diagonal d)
        i   = n - sum     (row index)
        j   = d - i       (column index)
   The function computes A(i,j) and returns it as seq_t. */

seq_t a(idx_t n) {
    /* 1. Locate the diagonal that contains index n. */
    idx_t d   = 0;      /* diagonal number */
    idx_t sum = 0;      /* total elements up to, but not including, diagonal d */

    while (sum + (d + 1) <= n) {   /* sum of first (d+1) diagonals is (d+1)(d+2)/2   */
        sum += d + 1;
        d++;
    }

    /* 2. Compute coordinates on this diagonal. */
    idx_t i = n - sum;   /* row index                */
    idx_t j = d - i;     /* column index             */

    /* 3. Evaluate A(i,j) by the recurrence. */
    if (j == 0) return (seq_t)0;
    if (j == 1) return (seq_t)i;

    seq_t prev = (seq_t)0;          /* A(i,0) */
    seq_t cur  = (seq_t)i;          /* A(i,1) */

    for (idx_t k = 2; k <= j; ++k) {
        seq_t next = ((seq_t)(4*i + 2) * cur) - prev + (seq_t)(2*i);
        prev = cur;
        cur  = next;
    }

    return cur;
}
