/*
 * A399833  The number of divisors d of n such that gcud(d, n/d) is a power of
 *          2, where gcud is the greatest common unitary divisor.
 * https://oeis.org/A399833
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Fixed-point formula: a(n) = (e2+1) * ∏_{p odd prime | n} (ep if ep even; ep+1 if ep odd),
   where n = ∏ p^ep and e2 is the exponent of 2 in n.  This is derived from the
   definition that a divisor d of n is counted iff the greatest common unitary
   divisor gcud(d, n/d) is a power of 2.  */

/* Compute a(n) for any n >= 1.  The function uses only integer arithmetic
   and simple trial division; it is efficient for n up to at least 1e12
   (and comfortably fast for the required range up to 1e4).  */
seq_t a(idx_t n)
{
    /* Count exponent of 2 */
    unsigned int e2 = 0;
    uint64_t tmp = n;
    while ((tmp & 1ULL) == 0) {
        tmp >>= 1;
        ++e2;
    }

    seq_t result = (seq_t)(e2 + 1);          /* include the 2-part */

    /* Handle odd prime factors */
    for (uint64_t p = 3; p * p <= tmp; p += 2) {
        if (tmp % p == 0) {
            unsigned int e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            result *= (seq_t)((e & 1) ? (e + 1) : e);
        }
    }

    /* If a prime factor greater than sqrt(tmp) remains (its exponent is 1) */
    if (tmp > 1) {
        result *= (seq_t)(2);   /* exponent 1 is odd, use e+1 = 2 */
    }

    return result;
}
