/*
 * A300330  a(n) is the product over all prime powers p^e where p^e is the
 *          highest power of p dividing n and p-1 does not divide n.
 * https://oeis.org/A300330
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product of p^e for each prime power p^e || n such that
 *            (p-1) does not divide n
 * Range:     u8 n <= 256, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    seq_t result = 1;
    idx_t temp = n;

    /* Remove factor 2 (always excluded because 1 | n) */
    while ((temp & 1ULL) == 0)
        temp >>= 1;

    /* Process odd primes */
    for (idx_t p = 3; p * p <= temp; p += 2) {
        idx_t e = 0;
        while (temp % p == 0) {
            temp /= p;
            ++e;
        }
        if (e > 0 && n % (p - 1) != 0) {
            seq_t pow = 1;
            for (idx_t i = 0; i < e; ++i)
                pow *= (seq_t)p;
            result *= pow;
        }
    }

    /* Remaining prime factor, if any */
    if (temp > 1) {
        if (n % (temp - 1) != 0)
            result *= (seq_t)temp;
    }

    return result;
}
