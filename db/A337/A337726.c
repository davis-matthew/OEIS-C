/*
 * A337726  a(n) = (3*n+2)! * Sum_{k=0..n} 1 / (3*k+2)!.
 * https://oeis.org/A337726
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (3*n+2)! * Sum_{k=0..n} 1/(3*k+2)!
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 6, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1; a(n)=((3*n)*(3*n+1)*(3*n+2))*a(n-1)+1 */

seq_t a(idx_t n)
{
    seq_t result = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i) {
        seq_t t = (seq_t)3 * i;
        seq_t prod = t * (t + (seq_t)1) * (t + (seq_t)2);
        result = prod * result + (seq_t)1;
    }
    return result;
}
