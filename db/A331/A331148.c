/*
 * A331148  Triangle read by rows: T(n,k) (n>=k>=1) = f(n,n-k+1) where f(n,k)
 *          = floor((n/k)*floor(n/k)).
 * https://oeis.org/A331148
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the n‑th entry of the triangle * T(r,c) = f(r, r−c+1)
 *            with * f(n,k) = floor((n/k)*floor(n/k)) = floor((n * floor(n/k))
 *            / k)
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS sequence A331148 (1‑based indexing). */
seq_t a(idx_t n)
{
    /* Determine the row r such that r(r+1)/2 >= n. */
    idx_t r   = 1;
    idx_t sum = 1;          /* sum = r*(r+1)/2 for current r */
    while (sum < n) {
        ++r;
        sum += r;
    }
    /* sum now equals r(r+1)/2; previous row's total is sum - r. */
    idx_t prev = sum - r;          /* (r-1)*r/2 */
    idx_t c    = n - prev;         /* column index (1‑based) */
    idx_t k    = r - c + 1;        /* argument to f */

    /* Compute f(r,k) safely without overflow. */
    idx_t m    = r / k;            /* floor(r/k) */
    idx_t rest = r % k;            /* r mod k */

    /* f(r,k) = m*m + floor((rest*m)/k) */
    seq_t term = (seq_t)m * (seq_t)m;
    term += (seq_t)rest * (seq_t)m / (seq_t)k;

    return term;
}
