/*
 * A330828  The squares of the Fermat primes, A019434(n)^2.
 * https://oeis.org/A330828
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, hard, more
 * Formula:   a(n) = (Fermat prime)^2
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;  /* undefined for n < 1 */

    /* Known Fermat primes */
    static const uint64_t primes64[] = {3ULL, 5ULL, 17ULL, 257ULL, 65537ULL};
    static const size_t count = sizeof primes64 / sizeof primes64[0];

    if ((idx_t)n > count)
        return 0;  /* no further known Fermat primes */

    seq_t p = (seq_t)primes64[n - 1];
    return p * p;
}
