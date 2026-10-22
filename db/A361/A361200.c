/*
 * A361200  Product of the left half (exclusive) of the multiset of prime
 *          factors of n; a(1) = 0.
 * https://oeis.org/A361200
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product of the left half (exclusive) of the multiset of
 *            prime factors of n; a(1) = 0
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;

    idx_t temp = n;        /* remaining factor to process */
    /* The largest number of prime factors for n <= 2^64 is < 64, so a fixed array is safe */
    int primes[64];
    int cnt = 0;

    /* Trial division */
    for (idx_t i = 2; i * i <= temp; ++i) {
        while (temp % i == 0) {
            primes[cnt++] = (int)i;
            temp /= i;
        }
    }
    if (temp > 1) {           /* remaining prime factor > sqrt(original) */
        primes[cnt++] = (int)temp;
    }

    int half = cnt / 2;
    seq_t prod = (seq_t)1;
    for (int i = 0; i < half; ++i) {
        prod *= (seq_t)primes[i];
    }

    return prod;
}
