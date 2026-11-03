/*
 * A329273  a(1)=1. If n is prime, a(n)=0; if not, a(n) = (the smallest prime
 *          number greater than n) minus (the largest prime number smaller
 *          than n) minus 1.
 * https://oeis.org/A329273
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n = 1; otherwise a(n) = 0 if n is prime; else a(n) =
 *            nextprime(n) - prevprime(n) - 1
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Validated: partial, pass, 100 terms through n = 100 (2026-11-01)
 * Helpers:   is_prime, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"
#include "../utils/next_prime.h"

/* Return the largest prime < n   (n > 2)   ; pp(2) = 1, but we never call it for n==2 */
static inline idx_t prev_prime(idx_t n)
{
    idx_t p = n - 1;
    for (;; --p)
    {
        if (is_prime(p)) return p;
    }
}

seq_t a(idx_t n)
{
    if (n == 1) {
        return (seq_t)1;
    }
    if (is_prime(n)) {
        return (seq_t)0;
    }
    idx_t p_prev = prev_prime(n);
    uint64_t p_next = next_prime(n);
    seq_t result = (seq_t)p_next - (seq_t)p_prev - (seq_t)1;
    return result;
}
