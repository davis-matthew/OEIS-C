/*
 * A253392  Number of (n+1) X (3+1) 0..1 arrays with every 2 X 2 subblock
 *          antidiagonal maximum minus diagonal minimum nondecreasing
 *          horizontally and diagonal maximum minus antidiagonal minimum
 *          nondecreasing vertically.
 * https://oeis.org/A253392
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=96, a(2)=143, a(3)=174, a(4)=246. For n>4 a(n) = 7*n^2 +
 *            18*n + 59 if n is even, 7*n^2 + 18*n + 51 if n is odd
 * Range:     u8 n <= 4, u16 n <= 95, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is easily computed from the explicit quadratic formula
   (valid for all n>4).  No recurrence or heavy computation is needed
   for n up to at least 10000. */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)96;
    if (n == 2) return (seq_t)143;
    if (n == 3) return (seq_t)174;
    if (n == 4) return (seq_t)246;

    /* Convert the index to seq_t before arithmetic to avoid
     * accidental promotion or overflow.  The multiplication
     * is carried out in seq_t, which is sufficient for all
     * test values. */
    seq_t nn = (seq_t)n;
    seq_t term = 7 * nn * nn + 18 * nn;
    return term + ((n & 1) ? (seq_t)51 : (seq_t)59);
}
