/*
 * A374156  Primes p that are quadratic residues modulo 59. Row 17 of A373751.
 * https://oeis.org/A374156
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   the nth prime p such that p is a quadratic residue modulo 59
 *            (i.e., Jacobi symbol (p|59) = 1, or p = 59)
 * Range:     u8 n <= 25, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   is_prime, jacobi_symbol
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/is_prime.h"
#include "../utils/jacobi_symbol.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    idx_t count = 0;
    idx_t p = 1;          /* start from the prime after 1 */
    while (count < n) {
        ++p;
        if (!is_prime(p)) continue;
        if (p == 59) {
            ++count;
            continue;
        }
        if (jacobi_symbol((uint64_t)p, 59) == 1) {
            ++count;
        }
    }
    return (seq_t)p;
}
