/*
 * A275989  a(n) = prime(prime(n)+1) - prime(n).
 * https://oeis.org/A275989
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(prime(n)+1) - prime(n)
 * Range:     u8 n <= 18, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   a(n) computes the difference between the prime at index (prime(n)+1)
    and the prime at index n.  The function uses the supplied helper
    get_prime(idx_t k) which returns the k‑th prime number.  */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;          /* Below the offset; behaviour undefined */

    /* Prime at index n  */
    uint32_t p_n = get_prime(n);

    /* Prime at index p_n + 1  */
    uint32_t idx_next = p_n + 1u;
    uint32_t p_next = get_prime(idx_next);

    /* Difference, expressed in seq_t  */
    return (seq_t)p_next - (seq_t)p_n;
}
