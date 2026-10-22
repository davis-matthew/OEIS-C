/*
 * A385610  Galileo sequence with ratio k = 2: a(1) = 1, a(2) = k, a(2*n-1) =
 *          floor(((k + 1)*a(n) -1)/2), and a(2*n) = floor((k + 1)*a(n)/2) + 1
 *          for n > 2.
 * https://oeis.org/A385610
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, look
 * Formula:   a(1)=1, a(2)=2, a(2*n-1)=floor(((k+1)*a(n)-1)/2),
 *            a(2*n)=floor((k+1)*a(n)/2)+1 (k=2)
 * Range:     u8 n <= 3691, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* base cases */
    if (n == 1) return 1;
    if (n == 2) return 2;

    /* odd index: n = 2*m-1  =>  m = (n+1)/2 */
    if (n & 1) {
        idx_t m = (n + 1) >> 1;          /* (n+1)/2 */
        seq_t sub = a(m);                /* recursive call */
        return ((seq_t)3 * sub - 1) / 2; /* floor((3*sub - 1)/2) */
    }
    /* even index: n = 2*m  =>  m = n/2 */
    {
        idx_t m = n >> 1;                /* n/2 */
        seq_t sub = a(m);                /* recursive call */
        return ((seq_t)3 * sub) / 2 + 1; /* floor(3*sub/2) + 1 */
    }
}
