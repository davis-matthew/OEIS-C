/*
 * A353557  a(n) = 1 if n is an odd number with an even number of prime
 *          factors (counted with multiplicity), otherwise 0.
 * https://oeis.org/A353557
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n is odd and the number of prime factors of n
 *            (counted with multiplicity) is even; otherwise 0
 * Range:     u8 n <= 9807, u16 n <= 9807, u32 n <= 9807, u64 n <= 9807,
 *            u128 n <= 9807, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Even indices cannot satisfy the condition */
    if ((n & 1U) == 0U)
        return (seq_t)0;

    uint64_t v = (uint64_t)n;
    int parity = 0;                    /* parity of the number of prime factors */

    /* Trial division up to sqrt(v) – no overflow on p*p */
    for (uint64_t p = 2; p <= v / p; ++p) {
        while (v % p == 0) {
            parity ^= 1;               /* toggle parity for each occurrence */
            v /= p;
        }
    }

    /* A remaining v > 1 is a prime factor > sqrt(original n) */
    if (v > 1)
        parity ^= 1;

    /* Even parity (parity == 0) → return 1, else 0 */
    return (parity == 0) ? (seq_t)1 : (seq_t)0;
}
