/*
 * A359768  a(n) = 1 if the parity of n and that of sopfr(n) differ, otherwise
 *          0. Here sopfr is the sum of prime factors (with repetition).
 * https://oeis.org/A359768
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if parity of n differs from parity of sopfr(n),
 *            otherwise 0
 * Range:     u8 n <= 9820, u16 n <= 9820, u32 n <= 9820, u64 n <= 9820,
 *            u128 n <= 9820, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    unsigned parity_n = n & 1U;       /* parity of n (0 even, 1 odd) */
    unsigned parity_sopfr = 0;        /* parity of the sum of prime factors with multiplicity */

    idx_t m = n;

    /* remove all factors 2 – they do not contribute to parity_sopfr */
    while ((m & 1U) == 0)
        m >>= 1U;

    /* factor odd primes */
    for (idx_t p = 3; p <= m / p; p += 2)
    {
        while (m % p == 0)
        {
            parity_sopfr ^= 1U;   /* toggle each ordinary prime factor */
            m /= p;
        }
    }

    /* if anything remains, it is an odd prime factor with exponent 1 */
    if (m > 1)
        parity_sopfr ^= 1U;

    return (seq_t)(parity_n ^ parity_sopfr); /* 1 if parities differ, else 0 */
}
