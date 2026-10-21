/*
 * A267068  a(n) = (n+1) / A189733(n).
 * https://oeis.org/A267068
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+1)/A189733(n)
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent closed form: a(n)=n+1 if n%6==0 or 4; 2 if n%6==1 or 3; 3 if n%6==2; 1 if n%6==5. */
seq_t a(idx_t n)
{
    switch (n % 6) {
        case 0:
        case 4:
            return ((seq_t)n) + 1;
        case 1:
        case 3:
            return (seq_t)2;
        case 2:
            return (seq_t)3;
        case 5:
            return (seq_t)1;
    }
    /* unreachable, but keep compiler quiet */
    return (seq_t)0;
}
