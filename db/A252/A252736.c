/*
 * A252736  a(1) = a(2) = 0; for n > 2: a(2n) = 1 + a(n), a(2n+1) =
 *          a(A064989(2n+1)).
 * https://oeis.org/A252736
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Ω(n) - 1 where Ω(n) is the number of prime factors of n
 *            counted with multiplicity. For n = 1 we define a(1) = 0. For any
 *            prime p, Ω(p)=1 so a(p)=0
 * Range:     u8 n <= 8192, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1)                /* a(1) = 0; also handles n = 0 just in case */
        return 0;

    /* Count prime factors with multiplicity (Ω(n)) */
    idx_t temp = n;
    unsigned int count = 0;    /* Ω(n) fits in unsigned int for n <= UINT64_MAX */

    /* factor 2 */
    while ((temp & 1) == 0) {
        ++count;
        temp >>= 1;            /* temp /= 2 */
    }

    /* factor odd primes */
    for (idx_t p = 3; p <= temp / p; p += 2) {
        while (temp % p == 0) {
            ++count;
            temp /= p;
        }
    }

    /* if anything remains, it is a prime factor > sqrt(n) */
    if (temp > 1)
        ++count;

    /* a(n) = Ω(n) - 1; subtract 1 only when Ω(n) > 0 which is always for n>1 */
    return (seq_t)(count - 1);
}
