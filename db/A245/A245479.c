/*
 * A245479  Numbers n such that the n-th cyclotomic polynomial has a root mod
 *          7.
 * https://oeis.org/A245479
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 46, u64 n <= 91, u128 n <= 183,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A245479: a(n) = d * 7^⌊(n‑1)/4⌋  where d∈{1,2,3,6} depends on n mod 4.
   Indexing starts at 1 (a(1)=1).  The four d‑values correspond to n≡1,2,3,0 (mod 4). */

seq_t a(idx_t n)
{
    /* Handle the (unreachable) case n==0 cheaply. */
    if (n == 0)
        return 0;

    /* Convert to 0‑based index so that j = floor(index/4). */
    idx_t idx = n - 1;
    idx_t j   = idx >> 2;        /* same as idx / 4 */
    idx_t r   = idx & 3;         /* same as idx % 4 */

    /* The multiplier d for each residue class. */
    static const seq_t d[4] = { (seq_t)1, (seq_t)2, (seq_t)3, (seq_t)6 };

    /* Compute 7^j by repeated multiplication.  Using the * operator
       works for both fixed‑width unsigned integers (wrap‑around) and
       arbitrary‑precision seq_t types. */
    seq_t pow7 = 1;
    for (idx_t i = 0; i < j; ++i)
        pow7 *= (seq_t)7;

    /* Final result. */
    return pow7 * d[r];
}
