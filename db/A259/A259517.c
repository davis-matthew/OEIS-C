/*
 * A259517  Number of (n+1) X (1+1) 0..1 arrays with each 2 X 2 subblock
 *          having clockwise pattern 0000 0001 0011 0101 or 1111.
 * https://oeis.org/A259517
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 18, u64 n <= 37, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:  a(1)=12, a(2)=38, a(3)=120,
    a(n) = 4*a(n-1) - 2*a(n-2) - 2*a(n-3)  for n ≥ 4. */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)12;
    if (n == 2) return (seq_t)38;
    if (n == 3) return (seq_t)120;

    /* a(n-3), a(n-2), a(n-1) */
    seq_t t1 = (seq_t)12;
    seq_t t2 = (seq_t)38;
    seq_t t3 = (seq_t)120;
    seq_t tn = 0;

    for (idx_t i = 4; i <= n; ++i) {
        tn = (seq_t)4 * t3 - (seq_t)2 * t2 - (seq_t)2 * t1;
        t1 = t2;
        t2 = t3;
        t3 = tn;
    }

    return tn;
}
