/*
 * A303479  Total volume of the family of rectangular prisms with dimensions
 *          p, q, and |q - p| where p divides q, n = p + q and p < q.
 * https://oeis.org/A303479
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{i=1..⌊(n-1)/2⌋} i*(n-i)*(n-2*i) if i divides n, else 0
 * Range:     u8 n <= 11, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Translated from OEIS A303479 */
seq_t a(idx_t n)
{
    seq_t sum = 0;
    if (n < 3) return 0;   /* trivial cases */
    idx_t limit = (n - 1) / 2;
    for (idx_t i = 1; i <= limit; ++i) {
        if (n % i == 0) {
            seq_t i_t = (seq_t)i;
            seq_t n_t = (seq_t)n;
            sum += i_t * (n_t - i_t) * (n_t - 2 * i_t);
        }
    }
    return sum;
}
