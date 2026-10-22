/*
 * A332174  a(n) = 7*(10^(2n+1)-1)/9 - 3*10^n.
 * https://oeis.org/A332174
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 4, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=4, a(1)=747, a(2)=77477;
   a(n)=111*a(n-1)-1110*a(n-2)+1000*a(n-3) for n>2 */
seq_t a(idx_t n)
{
    if (n == 0u) return (seq_t)4;
    if (n == 1u) return (seq_t)747;
    if (n == 2u) return (seq_t)77477;

    seq_t a0 = (seq_t)4;
    seq_t a1 = (seq_t)747;
    seq_t a2 = (seq_t)77477;

    for (idx_t i = 3; i <= n; ++i) {
        seq_t next = ((seq_t)111 * a2) -
                     ((seq_t)1110 * a1) +
                     ((seq_t)1000 * a0);
        a0 = a1;
        a1 = a2;
        a2 = next;
    }
    return a2;
}
