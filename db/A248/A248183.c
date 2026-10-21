/*
 * A248183  Least k such that 1/4 - Sum_{h = 1..k} 1/(h*(h+1)*(h+2)) < 1/n^2.
 * https://oeis.org/A248183
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = minimal k such that 2*(k+1)*(k+2) > n^2 (equivalently
 *            a(n)=floor((-1+sqrt(1+2*n^2))/2) for n>1)
 * Range:     u8 n <= 12, u16 n <= 315, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* a(1) is defined to be 1 */
    if (n <= 1) return (seq_t)1;

    /* Compute n^2 once (seq_t to avoid overflow in seq_t builds) */
    seq_t n_sq = (seq_t)n * (seq_t)n;

    /* Binary search on k in [1, n] for 2*(k+1)*(k+2) > n^2 */
    idx_t low  = 1;
    idx_t high = n;                /* inclusive range */

    while (low < high) {
        idx_t mid = low + (high - low) / 2;

        seq_t t1   = (seq_t)(mid + 1);
        seq_t t2   = (seq_t)(mid + 2);
        seq_t prod = t1 * t2;     /* (mid+1)*(mid+2) */
        prod *= (seq_t)2;          /* 2 * (mid+1)*(mid+2) */

        if (prod > n_sq)
            high = mid;          /* candidate is valid and possibly too large */
        else
            low = mid + 1;        /* need a larger k */
    }

    return (seq_t)low;            /* minimal k satisfying the inequality */
}
