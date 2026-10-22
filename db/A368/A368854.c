/*
 * A368854  a(n) = n! * (n+1)! * (n+2)! * Sum_{k=0..n} (-1)^k/(k! * (k+1)! *
 *          (k+2)!).
 * https://oeis.org/A368854
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(n+2)*a(n-1) + (-1)^n (a(0) = 1)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(0) */
    if (n == 0)
        return (seq_t)1;

    seq_t prev = (seq_t)1;          /* a(0) */
    for (idx_t i = 1; i <= n; ++i) {
        seq_t mult = (seq_t)i;
        mult *= ((seq_t)i + 1);
        mult *= ((seq_t)i + 2);
        if (i & 1)                 /* odd i: add (-1)^i = -1 */
            prev = mult * prev - (seq_t)1;
        else                       /* even i: add 1 */
            prev = mult * prev + (seq_t)1;
    }
    return prev;
}
