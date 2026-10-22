/*
 * A345238  a(n) is the number of primes of the form prime(n)+2^k for 2^k <
 *          prime(n).
 * https://oeis.org/A345238
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of primes of the form prime(n)+2^k where 2^k <
 *            prime(n)
 * Range:     u8 n <= 31, u16 n <= 4078, u32 n <= 4078, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"     /* get_prime(idx_t k) -> uint32_t */
#include "../utils/is_prime_big.h"  /* is_prime_big(seq_t n) -> bool */

seq_t a(idx_t n)
{
    if (n < 1) return 0;                     /* undefined for n < 1 */
    /* get the n-th prime (1‑based) */
    uint32_t p_u = get_prime(n);
    seq_t p = (seq_t)p_u;                    /* promote to seq_t for addition */

    seq_t count = 0;
    uint64_t pow = 1;                        /* 2^k as unsigned 64‑bit */
    while (pow < (uint64_t)p_u) {            /* stop when 2^k >= prime(n) */
        seq_t candidate = p + (seq_t)pow;
        if (is_prime_big(candidate)) {
            ++count;
        }
        pow <<= 1;                           /* next power of two */
    }
    return count;
}
