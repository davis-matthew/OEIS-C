/*
 * A267889  Binary representation of the n-th iteration of the "Rule 239"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A267889
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1; a(1)=110; a(2)=11111; a(3)=1111111;
   a(n)=101*a(n-1)-100*a(n-2) for n>3
   (Binary representation of the n‑th iteration of Rule 239 starting with a single ON cell) */

seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)110;
    if (n == 2) return (seq_t)11111;
    if (n == 3) return (seq_t)1111111;

    /* Iterative recurrence */
    seq_t a_nm2 = (seq_t)11111;     /* a(n-2) */
    seq_t a_nm1 = (seq_t)1111111;   /* a(n-1) */
    seq_t res = a_nm1;              /* placeholder for a(n) */

    for (idx_t i = 4; i <= n; ++i) {
        res = a_nm1 * (seq_t)101 - a_nm2 * (seq_t)100;
        a_nm2 = a_nm1;
        a_nm1 = res;
    }
    return res;
}
