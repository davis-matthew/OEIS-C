/*
 * A308108  Sum of the largest side lengths of all integer-sided scalene
 *          triangles with perimeter n.
 * https://oeis.org/A308108
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A308108: Sum of largest side lengths of all integer‑sided scalene
   triangles with perimeter n.  The sum is obtained by iterating over the
   possible largest side c.  For each c we count the number of admissible
   pairs (a,b) with a<b<c, a+b+c=n, and a+b>c; each such triangle contributes
   its largest side c to the sum. */
seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t c_max = (n - 1) / 2;           /* largest side must be < n/2       */
    for (idx_t c = 1; c <= c_max; ++c) {
        idx_t U = (n - c - 1) / 2;       /* maximum possible a for this c   */
        idx_t L = (n - 2 * c) + 1;       /* minimum a that keeps a<b<c      */
        if (L <= U) {
            seq_t cnt = (seq_t)(U - L + 1);   /* number of admissible a's      */
            sum += (seq_t)c * cnt;           /* add contribution of side c    */
        }
    }
    return sum;
}
