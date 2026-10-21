/*
 * A277132  The first subdiagonal of triangle A196842.
 * https://oeis.org/A277132
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1, a(2)=3, a(3)=14;
   a(n) + (-2*n-1)*a(n-1) + n^2*a(n-2) = 0   for n >= 4
   i.e.  a(n) = (2*n+1)*a(n-1) - n^2*a(n-2)   (note n is 1‑based) */

seq_t a(idx_t n)
{
    /* Handle base cases */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)3;
    if (n == 3) return (seq_t)14;

    /* Falling values for the recurrence */
    seq_t prev2 = (seq_t)3; /* a(n-2) when n=4 */
    seq_t prev1 = (seq_t)14;/* a(n-1) when n=4 */

    for (idx_t i = 4; i <= n; ++i) {
        /* a(i) = (2*i+1)*prev1 - i*i*prev2 */
        seq_t term = ((seq_t)(2*i + 1)) * prev1
                   - ((seq_t)i * (seq_t)i) * prev2;
        prev2 = prev1;
        prev1 = term;
    }
    return prev1;
}
