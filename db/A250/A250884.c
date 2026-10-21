/*
 * A250884  Number of (7+1)X(n+1) 0..3 arrays with nondecreasing
 *          x(i,j)-x(i,j-1) in the i direction and nondecreasing
 *          x(i,j)+x(i-1,j) in the j direction.
 * https://oeis.org/A250884
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (290548/3)*n^3 + 254464*n^2 + (669458/3)*n + 65536
 * Range:     u8 none, u16 none, u32 n <= 22, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This is rearranged to avoid fractional arithmetic:
   a(n) = (290548*n^3 + 763392*n^2 + 669458*n + 196608) / 3   */
/* Works for any seq_t that supports + - * / with integer types. */
seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;          /* Promote to seq_t for big‑int safety */
    seq_t n2 = n_seq * n_seq;        /* n^2 */
    seq_t n3 = n2 * n_seq;           /* n^3 */

    /* numerator = 290548*n^3 + 763392*n^2 + 669458*n + 196608 */
    seq_t num = (seq_t)290548 * n3;
    num += (seq_t)763392 * n2;
    num += (seq_t)669458 * n_seq;
    num += (seq_t)196608;

    /* exact division by 3 */
    return num / (seq_t)3;
}
