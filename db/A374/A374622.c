/*
 * A374622  Maximum number of vertices of a chordal ring mixed graph CRM(N,c)
 *          with diameter n.
 * https://oeis.org/A374622
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+1)^2/2 if n is odd a(n) = n^2/2 + 2 if n ≡ 0 (mod 4)
 *            a(n) = n*(n/2-1) + 4 if n ≡ 2 (mod 4) for n ≥ 3 Undefined for n
 *            < 3 (returns 0)
 * Range:     u8 n <= 22, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3)
        return 0; /* below offset */

    if (n & 1)  /* n odd */
    {
        seq_t t = (seq_t)(n + 1);
        return (t * t) / 2;
    }

    if ((n % 4) == 0)  /* n ≡ 0 (mod 4) */
    {
        seq_t t = (seq_t)n;
        return (t * t) / 2 + 2;
    }

    /* n even, n ≡ 2 (mod 4) */
    seq_t n_seq = (seq_t)n;
    seq_t half   = n_seq / 2;
    return n_seq * (half - 1) + 4;
}
