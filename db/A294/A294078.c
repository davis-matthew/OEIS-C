/*
 * A294078  a(n) is the smallest even number k such that k*prime(n) - 1 or
 *          k*prime(n) + 1 is prime.
 * https://oeis.org/A294078
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the smallest even k such that k*prime(n)-1 or
 *            k*prime(n)+1 is prime
 * Range:     u8 n <= 14, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   get_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/is_prime_big.h"

/* seq_t and idx_t are supplied by the build system. */
seq_t a(idx_t n)
{
    if (n == 0)   /* undefined for 0; return 0 to avoid UB */
        return 0;

    uint32_t p = get_prime(n);          /* prime(n), offset 1 */

    for (idx_t k = 2; ; k += 2) {
        seq_t v1 = (seq_t)k * (seq_t)p - 1;
        if (is_prime_big(v1))
            return (seq_t)k;

        seq_t v2 = (seq_t)k * (seq_t)p + 1;
        if (is_prime_big(v2))
            return (seq_t)k;
    }
}
