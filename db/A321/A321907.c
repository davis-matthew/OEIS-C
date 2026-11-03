/*
 * A321907  If n > 1 is the k-th prime number, then a(n) = k!, otherwise a(n)
 *          = 0.
 * https://oeis.org/A321907
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1; if n>1 is the k-th prime then a(n)=k!, otherwise a(n)=0
 * Range:     u8 n <= 12, u16 n <= 22, u32 n <= 40, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Validated: partial, pass, 58 terms through n = 58 (2026-11-01)
 * Helpers:   is_prime, prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/prime_pi.h"

/* Helper: compute factorials iteratively.
   For built‑in integer types the result may overflow for large n,
   but the tests use only n for which the value fits in seq_t. */
static seq_t factorial(idx_t n)
{
    seq_t result = 1;
    for (idx_t i = 2; i <= n; ++i)
        result *= (seq_t)i;
    return result;
}

seq_t a(idx_t n)
{
    /* a(1) is defined as 1 */
    if (n == 1) {
        return (seq_t)1;
    }

    /* For n > 1, check if n is prime */
    if (!is_prime(n)) {
        return (seq_t)0;
    }

    /* n is prime: its index k equals π(n) */
    seq_t pi_val = prime_pi(n);            /* π(n) as seq_t */
    idx_t k = (idx_t)pi_val;                /* convert to idx_t for factorial */

    return factorial(k);                    /* k! returned as seq_t */
}
