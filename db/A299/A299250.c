/*
 * A299250  Numbers congruent to {9, 11, 21, 29} mod 30.
 * https://oeis.org/A299250
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n)=a(n-4)+30 for n>4, with initial values a(1)=9, a(2)=11,
 *            a(3)=21, a(4)=29. Equivalent closed form: a(n)=base[(n-1)%4] +
 *            30*((n-1)/4)
 * Range:     u8 n <= 34, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* base residues for positions 1..4 modulo 4 */
    static const seq_t base[4] = {9, 11, 21, 29};

    /* n is guaranteed to be >=1 by the problem statement.
       If n==0 we simply return 0; behaviour for n<1 is undefined. */
    if (n == 0) return 0;

    /* 0‑based index for the residue cycle */
    idx_t idx = n - 1;
    idx_t q   = idx / 4;          /* number of full 30‑step cycles */
    idx_t r   = idx % 4;          /* position in the residue cycle */

    /* Convert to seq_t for arithmetic */
    seq_t cycle   = (seq_t)q;
    seq_t result  = base[r] + (30 * cycle);
    return result;
}
