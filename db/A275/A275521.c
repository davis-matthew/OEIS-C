/*
 * A275521  Number of (n+floor(n/2))-block bicoverings of an n-set.
 * https://oeis.org/A275521
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+⌊n/2⌋)-block bicoverings of an n-set which
 *            equals a(0)=1, a(1)=0, a(2k)= (2k-1)!! (product of odd numbers
 *            up to 2k-1), a(2k+1)= ((4k)/3) · (2k+1)!! (product of odd
 *            numbers up to 2k+1), where (2m-1)!! = ∏_{i=1}^{m} (2i-1)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 18, u64 n <= 32, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;

    /* Even n = 2k */
    if ((n & 1) == 0) {
        seq_t prod = (seq_t)1;
        /* multiply all odd numbers <= n-1 */
        for (idx_t i = 1; i <= n - 1; i += 2)
            prod *= (seq_t)i;
        return prod;
    }

    /* Odd n = 2k+1,  k >= 1 */
    idx_t k = (n - 1) / 2;           /* k = (n-1)/2 */
    seq_t prod = (seq_t)1;
    /* (2k+1)!! = product of odd numbers <= n */
    for (idx_t i = 1; i <= n; i += 2)
        prod *= (seq_t)i;

    /* prod is divisible by 3 for all k >= 1 */
    prod /= (seq_t)3;
    prod *= (seq_t)(4 * k);
    return prod;
}
