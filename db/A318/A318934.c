/*
 * A318934  Numbers whose binary expansion begins with exactly two 1's.
 * https://oeis.org/A318934
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 64, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers whose binary expansion begins with exactly two 1's (A318934).
   a(1) = 3.
   For n > 1 let n' = n - 1 and let k = ⌊log₂(n')⌋.
   Then a(n) = 5 * 2ᵏ + n'.  */
seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)3;

    idx_t nprime = n - 1;
    idx_t k = 0;
    while ((nprime >> k) > 1)
        k++;               /* find floor(log2(nprime)) */

    /* compute 5 * 2^k  +  nprime  */
    seq_t pow2k = ((seq_t)1) << k;
    seq_t result = (seq_t)5 * pow2k + (seq_t)nprime;

    return result;
}
