/*
 * A367805  a(1) = 0; for n > 1, a(n) is the least positive integer k for
 *          which k*prime(n) + 2 is prime.
 * https://oeis.org/A367805
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 1895, u32 n <= 1895, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS A367805
 * a(1) = 0; for n > 1, a(n) is the least positive integer k for which k*prime(n) + 2 is prime.
 * This implementation uses get_prime(idx_t) to obtain the n-th prime and
 * is_prime_big(seq_t) to test primality of k*prime(n)+2.
 */

#include "../utils/get_prime.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;               /* a(1) = 0 by definition */

    /* Get the n-th prime (1‑indexed).  get_prime expects n as idx_t. */
    uint32_t p32 = get_prime(n);
    seq_t p = (seq_t)p32;        /* promote to seq_t for multiplication */

    for (seq_t k = 1;; ++k) {    /* search for smallest positive k */
        seq_t m = k * p + 2;     /* candidate number */
        if (is_prime_big(m))
            return k;            /* found the least k */
    }
}
