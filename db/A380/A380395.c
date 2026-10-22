/*
 * A380395  The number of unitary divisors of n that are cubes.
 * https://oeis.org/A380395
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = number of unitary divisors of n that are cubes. a(p^e) =
 *            2 if e is divisible by 3, otherwise 1. Hence a(n) = 2^k where k
 *            is the number of distinct primes whose exponent in n is a
 *            multiple of 3
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined for n < 1 */
    uint64_t x = (uint64_t)n;              /* cast to native unsigned type for factoring */
    seq_t result = 1;                      /* multiplicative result starts at 1 */

    for (uint64_t p = 2; p * p <= x; ++p) {
        if (x % p == 0) {
            uint64_t exp = 0;
            while (x % p == 0) {
                ++exp;
                x /= p;
            }
            if (exp % 3 == 0) {
                result *= 2;              /* each prime with exponent %3==0 doubles the count */
            }
        }
    }

    if (x > 1) {
        /* x is a remaining prime factor with exponent 1.
           1 % 3 != 0, so it contributes no additional factor of 2. */
    }

    return result;
}
