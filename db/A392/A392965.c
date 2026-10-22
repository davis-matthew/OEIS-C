/*
 * A392965  Least number of vertices in a stepwise-irregular tree with at
 *          least one vertex of degree n.
 * https://oeis.org/A392965
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + n * Sum_{k=0..n-2} (n-2)!/k! (n ≥ 0)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first two terms directly */
    if (n == 0) return 1;
    if (n == 1) return 2;

    /* Compute the sum S = Σ_{k=0}^{n-2} (n-2)! / k!   efficiently */
    seq_t sum  = 1;  /* term for k = n-2 (value 1) */
    seq_t term = 1;  /* current term (n-2)! / k!  */

    /* Iterate k from n-2 down to 1,
       updating term = term * k  (since (n-2)! / (k-1)! = ((n-2)! / k!) * k) */
    for (idx_t k = n - 2; k >= 1; --k) {
        term *= (seq_t)k;
        sum += term;
    }
    /* sum now equals Σ_{k=0}^{n-2} (n-2)! / k! */

    seq_t result = (seq_t)n * sum;
    result += 1;
    return result;
}
