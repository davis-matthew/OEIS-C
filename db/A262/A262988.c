/*
 * A262988  Number of distinct primes, including n if prime, obtained by
 *          cyclically shifting the digits of n.
 * https://oeis.org/A262988
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of distinct primes among the cyclic rotations of
 *            the decimal digits of n
 * Range:     u8 n <= 1367, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digits, is_prime_big, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/digits.h"
#include "../utils/pow10.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n) {
    if (n == 0)  return 0;              /* undefined for n<1, but guard */
    size_t d = (size_t)digits(n);        /* number of decimal digits */
    if (d == 0) d = 1;

    seq_t prime_cnt = 0;
    seq_t primes[20];                     /* enough for any number within 64 bits (max 19 digits) */

    for (size_t k = 0; k < d; ++k) {
        seq_t pow_k   = pow10(k);        /* 10^k */
        seq_t pow_dk  = pow10(d - k);    /* 10^(d-k) */
        seq_t high    = ((seq_t)n) / pow_dk;
        seq_t low     = ((seq_t)n) % pow_dk;
        seq_t rot     = low * pow_k + high;

        if (is_prime_big(rot)) {
            bool seen = false;
            for (size_t j = 0; j < (size_t)prime_cnt; ++j) {
                if (rot == primes[j]) { seen = true; break; }
            }
            if (!seen) {
                primes[(size_t)prime_cnt++] = rot;
            }
        }
    }
    return prime_cnt;
}
