/*
 * A384836  a(n) = Sum_{k=0..floor(n/4)} |Stirling1(n,4*k)|.
 * https://oeis.org/A384836
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/4)} |Stirling1(n,4*k)|
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count permutations of n whose number of cycles is ≡0 (mod 4).  */
seq_t a(idx_t n)
{
    /* f[r] will hold the number of permutations of the current size with
       cycle count ≡ r (mod 4).  Initially for n=0 we have one empty
       permutation with 0 cycles.  */
    seq_t f[4] = {0, 0, 0, 0};
    seq_t nf[4];
    f[0] = 1;                  /* empty permutation */

    for (idx_t i = 1; i <= n; ++i) {
        seq_t factor = (seq_t)(i - 1);   /* (n-1) multiplier for inserting into an existing cycle */

        for (int r = 0; r < 4; ++r) {
            /* r-1 mod 4 gives the previous residue needed when the new
               element forms a new cycle. */
            int prev = (r + 3) % 4;  /* (r-1) mod 4 */
            nf[r] = f[prev] + factor * f[r];
        }

        /* copy nf back to f for next iteration */
        f[0] = nf[0];
        f[1] = nf[1];
        f[2] = nf[2];
        f[3] = nf[3];
    }

    return f[0];  /* permutations with cycle count ≡0 (mod 4) */
}
