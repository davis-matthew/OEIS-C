/*
 * A320116  a(n) = Product_{d|n, d>1} prime(1+(d mod 6)).
 * https://oeis.org/A320116
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{d|n, d>1} prime(1+(d mod 6))
 * Range:     u8 n <= 7, u16 n <= 29, u32 n <= 119, u64 n <= 839,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The first six primes are needed. */
    static const seq_t primes[6] = {
        (seq_t)2,  /* prime(1) */
        (seq_t)3,  /* prime(2) */
        (seq_t)5,  /* prime(3) */
        (seq_t)7,  /* prime(4) */
        (seq_t)11, /* prime(5) */
        (seq_t)13  /* prime(6) */
    };

    if (n <= 1u)
        return (seq_t)1;           /* a(1) = 1 */

    seq_t result = (seq_t)1;

    /* iterate over divisors d with 2 ≤ d ≤ √n, handling the pair n/d */
    for (idx_t d = 2; d <= n / d; ++d) {
        if (n % d == 0) {
            /* process divisor d */
            idx_t r = 1 + (d % 6);
            result *= primes[r - 1];   /* r ∈ {1,…,6} */

            /* process paired divisor n/d, if distinct from d */
            idx_t d2 = n / d;
            if (d2 != d) {
                idx_t r2 = 1 + (d2 % 6);
                result *= primes[r2 - 1];
            }
        }
    }

    /* process the divisor d = n itself (which is >1 for n>1) */
    {
        idx_t r = 1 + (n % 6);
        result *= primes[r - 1];
    }

    return result;
}
