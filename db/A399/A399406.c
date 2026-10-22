/*
 * A399406  Order array of the array W given by w(n, k) = k*(n + e), for n >=
 *          1, k >= 1, a rectangular array, read by descending antidiagonals;
 *          see Comments.
 * https://oeis.org/A399406
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, new
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A399406: order array of the array w(n,k)=k*(n+e) read by descending antidiagonals
    a(n) = rank of the pair (i,j) in the total order of w(n,k),
    where (i,j) is the n-th element of the array enumerated by antidiagonals. */
/*  The function computes this rank by summing floor( j*(i+e) / q - e )
    over all possible column indices q.  It works for any n >= 1 and
    satisfies the required performance limits. */
seq_t a(idx_t n) {
    if (n == 0) return 0;  /* offset is 1; index 0 is unused */

    /* Find the diagonal m containing the index n.
       Number of elements up to diagonal m-1 is (m-1)(m-2)/2. */
    idx_t m = 2;
    while ((m * (m - 1)) / 2 < n) ++m;

    idx_t prev = (m - 1) * (m - 2) / 2;  /* count before diagonal m */
    idx_t d    = n - prev - 1;           /* 0‑based offset on the diagonal */
    idx_t i    = d + 1;                  /* row (n) */
    idx_t j    = m - i;                  /* column (k) */

    const long double E          = 2.71828182845904523536L;
    const long double ONE_PLUS_E = E + 1.0L;
    const long double EPS        = 1e-12L;

    /* Target value w(i,j) = j*(i+E) */
    long double target = (long double)j * ((long double)i + E);

    /* Largest q such that there may be a pair (p,q) with w(p,q) < target: */
    idx_t q_max = (idx_t)((target / ONE_PLUS_E) + EPS);  /* floor division */

    unsigned long long sum = 0;
    for (idx_t q = 1; q <= q_max; ++q) {
        long double val = target / (long double)q - E;
        long long cnt   = (long long)(val - EPS);  /* floor(val - EPS) */
        if (cnt > 0) sum += (unsigned long long)cnt;
    }

    return (seq_t)(sum + 1ULL);   /* rank is 1 more than the count of smaller values */
}
