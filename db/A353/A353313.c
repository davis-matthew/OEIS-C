/*
 * A353313  If n is of the form 3k, then a(n) = k, and if n is of the form
 *          3k+r, with r = 1 or 2, then a(n) = 5*k + 3 + r.
 * https://oeis.org/A353313
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n/3 if n%3==0 else 5*(n/3)+3+(n%3) (offset 0)
 * Range:     u8 n <= 153, u16 n <= 9891, u32 n <= 9891, u64 n <= 9891,
 *            u128 n <= 9891, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute integer division and remainder with the index type */
    idx_t k = n / 3;
    idx_t r = n % 3;

    /* cast to seq_t for the arithmetic */
    seq_t k_seq = (seq_t)k;
    seq_t r_seq = (seq_t)r;

    if (r == 0)
        return k_seq;
    else
        return (5 * k_seq) + 3 + r_seq;
}
