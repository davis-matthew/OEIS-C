/*
 * A339254  a(n) = 6*a(n - 1) - 12*a(n - 2) + 8*a(n - 3) for n >= 5, a(0) = 1,
 *          a(1) = 7, a(2) = 24, a(3) = 70, a(4) = 193.
 * https://oeis.org/A339254
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 24, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A339254
   Recurrence: a(n) = 6*a(n-1) - 12*a(n-2) + 8*a(n-3)  for n >= 5
   Initial values: a(0)=1, a(1)=7, a(2)=24, a(3)=70, a(4)=193
*/
seq_t a(idx_t n)
{
    /* handle small indices directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)7;
    if (n == 2) return (seq_t)24;
    if (n == 3) return (seq_t)70;
    if (n == 4) return (seq_t)193;

    /* rolling recurrence: keep last three terms */
    seq_t a0 = (seq_t)1;   /* a(n-3) */
    seq_t a1 = (seq_t)7;   /* a(n-2) */
    seq_t a2 = (seq_t)24;  /* a(n-1) */
    seq_t a3 = (seq_t)70;  /* a(n)  */
    seq_t a4 = (seq_t)193; /* a(n+1) */

    for (idx_t i = 5; i <= n; ++i) {
        seq_t next = (seq_t)6 * a4 - (seq_t)12 * a3 + (seq_t)8 * a2;
        a2 = a3;
        a3 = a4;
        a4 = next;
    }
    return a4;
}
