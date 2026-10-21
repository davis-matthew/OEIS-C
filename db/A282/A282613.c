/*
 * A282613  Number of inequivalent 3 X 3 matrices with entries in {1,2,3,..,n}
 *          up to rotations.
 * https://oeis.org/A282613
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^3*(n^2+1)*(n^4-n^2+2)/4
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 13, u64 n <= 161, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the index once. */
    seq_t nseq = (seq_t)n;

    /* Compute powers of n. */
    seq_t n2 = nseq * nseq;          /* n^2 */
    seq_t n3 = n2 * nseq;            /* n^3 */
    seq_t n4 = n2 * n2;              /* n^4 */

    /* Factors of the product. */
    seq_t f[3];
    f[0] = n3;            /* n^3 */
    f[1] = n2 + (seq_t)1; /* n^2 + 1 */
    f[2] = n4 - n2 + (seq_t)2; /* n^4 - n^2 + 2 */

    /* Divide by 4 by removing two factors of 2 from the factors. */
    int divs = 0;
    for (int i = 0; i < 3 && divs < 2; ++i)
    {
        while (divs < 2 && (f[i] & (seq_t)1) == (seq_t)0)
        {
            f[i] >>= 1;     /* divide by 2 */
            ++divs;
        }
    }
    /* In the unlikely event that we still have not removed two 2's,
       divide them from the first factor (e.g., when all factors were 0). */
    while (divs < 2)
    {
        f[0] >>= 1;
        ++divs;
    }

    /* Multiply the remaining factors. */
    seq_t result = (seq_t)1;
    for (int i = 0; i < 3; ++i)
        result *= f[i];

    return result;
}
