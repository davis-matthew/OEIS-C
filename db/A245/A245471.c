/*
 * A245471  If n is odd, then a(n) = A065621(n+1). If n is even, then a(n) =
 *          n/2.
 * https://oeis.org/A245471
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = n/2, if n is even; otherwise * a(n) = (n+1) XOR (((n+1) &
 *            n) << 1) * The second case implements the value A065621(n+1) for
 *            odd n
 * Range:     u8 n <= 128, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;          /* cast index to seq_t */
    if ((x & 1) == 0)            /* even n */
        return x / 2;

    /* odd n: compute A065621(n+1) */
    seq_t k = x + 1;              /* k = n+1, guaranteed even */
    seq_t t = k & (k - 1);        /* k & (k-1) is k with its lowest set bit cleared */
    return k ^ (t << 1);          /* k xor (t<<1) produces the desired value */
}
