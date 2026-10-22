/*
 * A359802  a(n) = product prime(d + 1), where d ranges over all the decimal
 *          digits of n.
 * https://oeis.org/A359802
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = product of prime(d+1) over the decimal digits d of n
 * Range:     u8 n <= 48, u16 n <= 1998, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Map a decimal digit d to the prime number at index d+1.
       prime(1)=2, prime(2)=3, ..., prime(10)=29. */
    static const seq_t primes[10] = {
        (seq_t)2,  (seq_t)3,  (seq_t)5,  (seq_t)7,
        (seq_t)11, (seq_t)13, (seq_t)17, (seq_t)19,
        (seq_t)23, (seq_t)29
    };

    /* Special case: n == 0 has a single digit '0'. */
    if (n == 0)
        return primes[0];

    seq_t result = 1;
    while (n > 0)
    {
        unsigned digit = (unsigned)(n % 10);
        result *= primes[digit];
        n /= 10;
    }
    return result;
}
