/*
 * A386278  Multiplicative sequence a(n) with a(p^e) = 1 + (e mod 4) * (3 - (e
 *          mod 4)) for prime p and e >= 0.
 * https://oeis.org/A386278
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = 3^m where m = number of prime exponents ≡ 1 or 2 (mod 4)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _smallest_prime_factor, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is multiplicative with a(p^e) = 1 + (e mod 4) * (3 - (e mod 4)) */

#include <stdint.h>
#include "../utils/_smallest_prime_factor.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n) {
    /* Count exponents with e mod 4 == 1 or 2 */
    idx_t k = 0;
    uint64_t m = n;
    while (m > 1) {
        uint64_t p = _smallest_prime_factor(m);
        idx_t e = 0;
        while (m % p == 0) {
            m /= p;
            ++e;
        }
        idx_t r = e % 4;
        if (r == 1 || r == 2) ++k;
    }
    /* Return 3^k */
    return seq_pow((seq_t)3, k);
}
