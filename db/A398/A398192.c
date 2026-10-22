/*
 * A398192  Triangle read by rows: T(n, k) = n! * Sum_{j=0..n-k} n^j / j!.
 * https://oeis.org/A398192
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where r = largest integer with r*(r+1)/2 <= n k =
 *            n - r*(r+1)/2 and T(r,k) = r! * sum_{j=0}^{r-k} r^j / j!
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 54, u64 n <= 135, u128 n <= 377,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* find row r (0‑based) */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;

    /* column k and remaining sum length d = r - k */
    idx_t offset = r * (r + 1) / 2;
    idx_t k = n - offset;
    idx_t d = r - k;

    /* factorial of r */
    seq_t fact = 1;
    for (idx_t i = 1; i <= r; ++i)
        fact *= (seq_t)i;

    /* compute T(r,k) using integer recurrence:
       term_j = (r! / j!) * r^j  (integer)
       sum over j=0..d */
    seq_t term = fact;
    seq_t sum = term;
    for (idx_t j = 1; j <= d; ++j)
    {
        term = (term * (seq_t)r) / (seq_t)j;
        sum += term;
    }

    return sum;
}
