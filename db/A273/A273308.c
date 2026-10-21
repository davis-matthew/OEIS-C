/*
 * A273308  Maximum population of a 2 X n still life in Conway's Game of Life.
 * https://oeis.org/A273308
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=0; for n>=2 if n%3==0: a(n)=4*(n/3); if n%3==1:
 *            a(n)=4*((n-1)/3)+2; if n%3==2: a(n)=4*((n+1)/3)
 * Range:     u8 n <= 190, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)0;
    switch (n % 3) {
        case 0: return (seq_t)4 * ((seq_t)(n / 3));
        case 1: return (seq_t)4 * ((seq_t)((n - 1) / 3)) + (seq_t)2;
        case 2: return (seq_t)4 * ((seq_t)((n + 1) / 3));
    }
    return (seq_t)0; /* unreachable, defensive */
}
