/*
 * A347941  For sets of n random points in the real plane, a(n) is an upper
 *          bound for the minimal number of nearest neighbors.
 * https://oeis.org/A347941
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2)=a(3)=2. For n > 3, let j = floor((n+5)/9) r = (n+5) mod 9
 *            if r < 6 a(n) = 2*j else a(n) = 2*j + 1 This is equivalent to
 *            a(n) = 2*floor((n+5)/9) if (n+5)%9 < 6 2*floor((n+5)/9)+1
 *            otherwise
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the initial terms where the formula does not apply */
    if (n < 4)
        return 2;

    /* Compute j = floor((n+5)/9) and r = (n+5) mod 9 */
    /* All operations are on |seq_t| except the cast from |idx_t| */
    seq_t sum = (seq_t)n + (seq_t)5;
    seq_t j   = sum / (seq_t)9;
    seq_t r   = sum % (seq_t)9;

    /* r < 6 ? 2*j : 2*j + 1 */
    if (r < (seq_t)6)
        return j * (seq_t)2;
    else
        return j * (seq_t)2 + (seq_t)1;
}
