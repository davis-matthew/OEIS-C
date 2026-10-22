/*
 * A400348  Array read by ascending antidiagonals: A(n,k) = [x^n] 1/(1 -
 *          k^2*x/(1 - x))^(n/k), with k > 0.
 * https://oeis.org/A400348
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, new
 * Formula:   a(n) = A(i,j) read by ascending anti‑diagonals of the table
 *            A(n,k) defined by A(0,k)=1 and for n>0 A(n,k) = [x^n] (1 -
 *            k^2*x/(1 - x))^{-n/k}. The mapping from the sequence index n to
 *            table indices (i,j) is i = s-1 - r , j = r+1 , where s is the
 *            smallest integer with s*(s+1)/2 > n and r = n - s*(s...
 * Range:     u8 n <= 9, u16 n <= 21, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Special case n = 0.  */
    if (n == 0) return (seq_t)1;

    /* Find the anti‑diagonal s that contains index n.
       We need the smallest s with s*(s+1)/2 > n. */
    idx_t s = 1;
    idx_t cum = 1;                    /* S(s) = s*(s+1)/2  ,  s starts at 1  */
    while (cum <= n) {
        ++s;
        cum += s;                      /* cum now equals S(s) */
    }

    /* Index r of the element inside the s‑th anti‑diagonal.  */
    idx_t prev_cum = s * (s - 1) / 2;  /* S(s-1)  */
    idx_t r = n - prev_cum;           /* 0‑based offset within the diagonal */

    /* Convert to table coordinates.  */
    idx_t i = s - 1 - r;              /* row index  (i >= 0) */
    idx_t j = r + 1;                  /* column index (j >= 1) */

    /* Compute A(i,j) where i = n and j = k.  */
    if (i == 0) return (seq_t)1;      /* A(0,k) = 1 for all k */

    seq_t sum = 0;
    seq_t Tprev = (seq_t)i * (seq_t)j;   /* j = 1 term: n*k */
    sum += Tprev;

    for (idx_t jj = 2; jj <= i; ++jj) {
        /* T_j = T_{j-1} * (i - j + 1) * (i + j*k - k) * k / (j*(j-1))  */
        seq_t numerator = Tprev
                         * (seq_t)(i - jj + 1)
                         * (seq_t)(i + j * (jj - 1))  /* j = k fixed above */
                         * (seq_t)j;
        idx_t denom = jj * (jj - 1);
        Tprev = numerator / (seq_t)denom;
        sum += Tprev;
    }

    return sum;
}
