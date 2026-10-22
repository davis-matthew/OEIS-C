/*
 * A333988  Square array T(n,k), n >= 0, k >= 0, read by antidiagonals, where
 *          column k is the expansion of (1-(k+1)*x) /
 *          (1-2*(k+1)*x+((k-1)*x)^2).
 * https://oeis.org/A333988
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the element obtained by reading the square array *
 *            T(n,k) over antidiagonals. * For fixed k, T(n,k) satisfies *
 *            T(0,k) = 1 * T(1,k) = k+1 * T(n,k) = 2*(k+1)*T(n-1,k) -
 *            (k-1)^2*T(n-2,k) for n>1 * This function returns the n‑th term
 *            of the sequence A333988
 * Range:     u8 n <= 24, u16 n <= 50, u32 n <= 129, u64 n <= 341,
 *            u128 n <= 932, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Find diagonal d such that n lies on it:
       cumulative terms up to diagonal d is (d+1)(d+2)/2. */
    idx_t d = 0;
    for (;;) {
        unsigned __int128 val = (unsigned __int128)(d + 1) * (d + 2) / 2;
        if (val > n) break;
        ++d;
    }

    unsigned __int128 prev_trig = (unsigned __int128)d * (d + 1) / 2;
    idx_t offset = n - (idx_t)prev_trig;   /* 0‑based offset on diagonal */
    idx_t i = offset;                     /* row index in T             */
    idx_t j = d - offset;                 /* column index in T          */

    if (i == 0) return (seq_t)1;
    if (i == 1) return (seq_t)(j + 1);

    seq_t k_seq  = (seq_t)j;          /* current column as seq_t    */
    seq_t prev_prev = (seq_t)1;       /* T(0,j)                      */
    seq_t prev_val  = k_seq + (seq_t)1;/* T(1,j)                      */
    seq_t cur = prev_val;             /* placeholder for result      */

    for (idx_t r = 2; r <= i; ++r) {
        /* (k-1)^2 = k^2 - 2k + 1, avoids negative values */
        seq_t km1_sq = k_seq * k_seq - (seq_t)2 * k_seq + (seq_t)1;
        seq_t term1   = (seq_t)2 * (k_seq + (seq_t)1) * prev_val;
        seq_t term2   = km1_sq * prev_prev;
        cur = term1 - term2;
        prev_prev = prev_val;
        prev_val = cur;
    }
    return cur;
}
