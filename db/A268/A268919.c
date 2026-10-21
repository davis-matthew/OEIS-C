/*
 * A268919  Numerators of the rational number triangle R(n, k) = (n^4 -
 *          30*n^2*k^2 + 60*n*k^3 -30*k^4) / (120*n), n >= 1, k = 1, ..., n.
 *          This is a regularized Sum_{j >= 0} (k + n*j)^(-s) for s = -3
 *          defined by analytic continuation of a generalized Hurwitz zeta
 *          function.
 * https://oeis.org/A268919
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, frac, tabl, easy
 * Formula:   a(n) is the numerator of the reduced fraction R(r,k) = (r^4 - 30
 *            r^2 k^2 + 60 r k^3 - 30 k^4) / (120 r) where (r,k) are derived
 *            from the index n as follows: r = smallest integer with r(r+1)/2
 *            ≥ n k = n - r(r-1)/2 (1 ≤ k ≤ r) The result is the reduced
 *            numerator after canceling the gcd with the den...
 * Range:     i8 n <= 1, i16 n <= 135, i32 n <= 1275, i64 n <= 1275,
 *            i128 n <= 1275, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find the row r: smallest r with r(r+1)/2 >= n. */
    idx_t r = 1;
    idx_t sum = 1;                     /* 1+2+...+r  */
    while (sum < n) {
        ++r;
        sum += r;
    }
    idx_t prev = sum - r;              /* total terms before row r */
    idx_t k = n - prev;                /* column index (1‑based) */

    seq_t row = (seq_t)r;
    seq_t col = (seq_t)k;

    /* Compute the numerator N = r^4 - 30 r^2 k^2 + 60 r k^3 - 30 k^4   */
    seq_t r2 = row * row;              /* r^2  */
    seq_t k2 = col * col;              /* k^2  */
    seq_t r4 = r2 * r2;                /* r^4  */

    seq_t k3 = k2 * col;               /* k^3  */
    seq_t k4 = k3 * col;               /* k^4  */

    seq_t t1 = r2 * k2;                /* r^2 k^2  */
    seq_t N  = r4
             - ((seq_t)30) * t1
             + ((seq_t)60) * row * k3
             - ((seq_t)30) * k4;

    /* Denominator D = 120 r  */
    seq_t D = ((seq_t)120) * row;

    /* Reduce N/D by GCD. */
    seq_t g = N >= (seq_t)0 ? N : -N;  /* |N|  */
    seq_t d = D;
    while (d != (seq_t)0) {
        seq_t rmod = g % d;
        g = d;
        d = rmod;
    }
    return N / g;                      /* reduced numerator */
}
