/*
 * A332749  a(n) is the sum of the residues of the first k positive triangular
 *          numbers modulo n, where k = n if n is odd, 2n if n is even.
 * https://oeis.org/A332749
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{i=1}^{k} ((i*(i+1)/2) mod n) where k = n if n is
 *            odd, k = 2*n if n is even
 * Range:     u8 n <= 19, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the n-th term of OEIS sequence A332749.  */
seq_t a(idx_t n)
{
    /* Determine the number of triangular numbers to use. */
    idx_t k = (n & 1U) ? n : 2 * n;

    seq_t sum = 0;          /* running sum of residues */
    idx_t t_mod = 0;        /* current triangular number mod n */

    /* Iterate through the first k triangular numbers. */
    for (idx_t i = 1; i <= k; ++i)
    {
        t_mod = (t_mod + i) % n;   /* T_i mod n, using T_i = T_{i-1}+i */
        sum += (seq_t)t_mod;       /* add residue to the sum */
    }
    return sum;
}
