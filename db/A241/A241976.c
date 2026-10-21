/*
 * A241976  Values of k such that k^2 + (k+3)^2 is a square.
 * https://oeis.org/A241976
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*a(n-1) - 7*a(n-2) + a(n-3) with initial values a(1)=0,
 *            a(2)=9, a(3)=60
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined for offset 1 – return 0 */
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)9;
    if (n == 3) return (seq_t)60;

    seq_t a1 = (seq_t)0;   /* a(n-3) */
    seq_t a2 = (seq_t)9;   /* a(n-2) */
    seq_t a3 = (seq_t)60;  /* a(n-1) */
    seq_t next;

    for (idx_t i = 4; i <= n; ++i) {
        next = (seq_t)(7 * a3 - 7 * a2 + a1);
        a1 = a2;
        a2 = a3;
        a3 = next;
    }
    return a3;
}
