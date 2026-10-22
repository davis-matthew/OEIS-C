/*
 * A386707  a(n) = Lucas(n) + 6.
 * https://oeis.org/A386707
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A000032(n) + 6
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Lucas numbers: L(0)=2, L(1)=1, L(k)=L(k-1)+L(k-2).  a(n)=L(n)+6. */
seq_t a(idx_t n)
{
    /* handle base cases */
    if (n == 0) {
        return (seq_t)2 + (seq_t)6;
    }
    seq_t prev2 = (seq_t)2; /* L(0) */
    seq_t prev1 = (seq_t)1; /* L(1) */
    if (n == 1) {
        return prev1 + (seq_t)6;
    }
    for (idx_t i = 2; i <= n; ++i) {
        seq_t next = prev2 + prev1;
        prev2 = prev1;
        prev1 = next;
    }
    return prev1 + (seq_t)6;
}
