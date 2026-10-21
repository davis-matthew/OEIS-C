/*
 * A291245  p-INVERT of (0,1,0,1,0,1,...), where p(S) = 1 - 5 S + S^2.
 * https://oeis.org/A291245
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 26, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A291245
   Recurrence: a(n) = 5*a(n-1) + a(n-2) – 5*a(n-3) – a(n-4)   for n ≥ 5
   Initial values: a(0)=5, a(1)=24, a(2)=120, a(3)=599, a(4)=2990 */

seq_t a(idx_t n)
{
    /* Handle the first five terms directly */
    if (n == 0) return (seq_t)5;
    if (n == 1) return (seq_t)24;
    if (n == 2) return (seq_t)120;
    if (n == 3) return (seq_t)599;
    if (n == 4) return (seq_t)2990;

    /* Rolling computation for n ≥ 5 */
    seq_t a0 = (seq_t)5;     /* a(n-4)  */
    seq_t a1 = (seq_t)24;    /* a(n-3)  */
    seq_t a2 = (seq_t)120;   /* a(n-2)  */
    seq_t a3 = (seq_t)599;   /* a(n-1)  */
    seq_t a4 = (seq_t)2990;  /* a(n)    */

    for (idx_t i = 5; i <= n; ++i) {
        seq_t next = ((seq_t)5 * a4) + a3 - ((seq_t)5 * a2) - a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = next;
    }
    return a4;
}
