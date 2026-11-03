/*
 * A388018  a(n) = A276086(floor(sigma(n)/2)), where A276086 is the primorial
 *          base exp-function, and sigma is the sum of divisors function.
 * https://oeis.org/A388018
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A276086(floor(sigma(n)/2)) where A276086(n) = Π p_i^d_i,
 *            with n written in primorial base (n = Σ d_i * Π_{j<i} p_j, 0 ≤
 *            d_i < p_{i+1})
 * Range:     u8 n <= 19, u16 n <= 83, u32 n <= 431, u64 n <= 3503,
 *            u128 n <= 11550, big any n
 * Validated: standard, pass, 84 terms through n = 9830 (2026-11-01)
 * Helpers:   ipow, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Includes from utils */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"
#include "../utils/next_prime.h"

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

#include <stdint.h>   /* for uint64_t */

seq_t a(idx_t n)
{
    /* m = floor(sigma(n)/2) */
    seq_t m = sigma_of(n) / (seq_t)2;

    if (m == 0)                    /* a(1) = 1 */
        return (seq_t)1;

    seq_t tmp   = m;               /* working copy of m               */
    seq_t p     = (seq_t)2;        /* first prime in the representation */
    seq_t result = (seq_t)1;       /* product of prime powers          */

    while (tmp != 0) {
        seq_t digit = tmp % p;     /* digit in the current position  */
        if (digit != 0) {
            seq_t p_pow = ipow(p, digit);
            result *= p_pow;
        }
        tmp /= p;                  /* remove current digit contribution */

        if (tmp == 0)
            break;                 /* finished representation        */

        /* move to next prime */
        uint64_t next = next_prime((uint64_t)p);
        p = (seq_t)next;
    }

    return result;
}
