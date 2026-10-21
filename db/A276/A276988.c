/*
 * A276988  a(n) is the least k such that 10*k+prime(n) is composite.
 * https://oeis.org/A276988
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k such that 10*k + prime(n) is composite
 * Range:     u8 n <= 52, u16 n <= 7582, u32 n <= 7582, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses get_prime to obtain the nth prime and
 * searches incrementally for the smallest k that yields a composite
 * number.  The search is limited to small k (practically ≤3 for n>2),
 * but the loop is kept general. */
#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* Get the nth prime; get_prime returns a uint32_t whose value
     * fits into seq_t for all n considered here. */
    seq_t p = (seq_t)get_prime(n);

    seq_t k = 1;
    for (;; ++k) {
        seq_t candidate = 10 * k + p;   /* candidate = 10*k + prime(n) */
        if (!is_prime_big(candidate)) {  /* composite if not prime */
            return k;
        }
    }
}
