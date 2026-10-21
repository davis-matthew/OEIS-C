/*
 * A241472  a(n) = |{0 < k < sqrt(prime(n)): k^2 + 1 is a quadratic residue
 *          modulo prime(n)}|.
 * https://oeis.org/A241472
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |{ 0 < k < sqrt(prime(n)) : k^2 + 1 is a quadratic
 *            residue modulo prime(n) }|
 * Range:     u8 n <= 18, u16 n <= 1685, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt, get_prime, jacobi_symbol
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/floor_sqrt.h"
#include "../utils/jacobi_symbol.h"

seq_t a(idx_t n)
{
    /* Get the nth prime (prime(n)) */
    uint32_t p = get_prime(n);

    /* Upper bound for k: floor(sqrt(p)) */
    idx_t limit = (idx_t)floor_sqrt((seq_t)p);

    seq_t count = 0;
    for (idx_t k = 1; k <= limit; ++k) {
        seq_t val = (seq_t)k * k + 1;          /* k^2 + 1 (fits in seq_t) */
        /* val modulo p fits in uint64_t as val <= p+1 */
        if (jacobi_symbol((uint64_t)val, (uint64_t)p) > 0) {
            ++count;
        }
    }
    return count;
}
