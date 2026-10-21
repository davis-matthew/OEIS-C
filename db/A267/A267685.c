/*
 * A267685  Decimal representation of the n-th iteration of the "Rule 203"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A267685
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*a(n‑1) – 14*a(n‑2) + 8*a(n‑3) * Initial values: a(0)=1,
 *            a(1)=4, a(2)=27, a(3)=119, a(4)=495
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first five terms directly */
    if (n == (idx_t)0) return (seq_t)1;
    if (n == (idx_t)1) return (seq_t)4;
    if (n == (idx_t)2) return (seq_t)27;
    if (n == (idx_t)3) return (seq_t)119;
    if (n == (idx_t)4) return (seq_t)495;

    /* Recurrence variables: a(n‑3), a(n‑2), a(n‑1) */
    seq_t a_nm3 = (seq_t)27;   /* a(2)   */
    seq_t a_nm2 = (seq_t)119;  /* a(3)   */
    seq_t a_nm1 = (seq_t)495;  /* a(4)   */

    for (idx_t i = (idx_t)5; i <= n; ++i) {
        seq_t next = (seq_t)((seq_t)7 * a_nm1
                             - (seq_t)14 * a_nm2
                             + (seq_t)8 * a_nm3);
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = next;
    }

    return a_nm1;
}
