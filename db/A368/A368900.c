/*
 * A368900  LCM-transform of Doudna sequence.
 * https://oeis.org/A368900
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1. For n > 1, if n-1 can be written as 2^i − 2^j (i ≥ j ≥
 *            0) then a(n) = prime(j+1); otherwise a(n) = 1. (Here prime(k)
 *            denotes the k‑th prime, i.e. 2,3,5,7,11,...) This follows from
 *            the property that a(n) = A014963(A005940(n))
 * Range:     u8 n <= 8541, u16 n <= 8541, u32 n <= 8541, u64 n <= 8541,
 *            u128 n <= 8541, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* First 70 primes: sufficient for all n that can be represented as 2^i−2^j
   (j ≤ 63 for 64‑bit indices). */
static const seq_t _primes[] = {
    2U,   3U,   5U,   7U,  11U, 13U, 17U, 19U, 23U, 29U,
   31U,  37U,  41U,  43U,  47U, 53U, 59U, 61U, 67U, 71U,
   73U,  79U,  83U,  89U,  97U,101U,103U,107U,109U,113U,
  127U,131U,137U,139U,149U,151U,157U,163U,167U,173U,
  179U,181U,191U,193U,197U,199U,211U,223U,227U,229U,
  233U,239U,241U,251U,257U,263U,269U,271U,277U,281U,
  283U,293U,307U,311U,313U,317U,331U,337U,347U,349U
};

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    /* n-1 is the number we test for 2^i - 2^j */
    idx_t x = n - 1;
    /* count trailing zeros -> j */
    idx_t j = 0;
    while ((x & 1) == 0) {
        x >>= 1;
        ++j;
    }
    /* x is now the odd part; check if odd+1 is a power of two */
    idx_t odd_plus = x + 1;
    if ((odd_plus & (odd_plus - 1)) == 0) {
        /* Return the (j+1)-th prime (1‑based) */
        return _primes[j];
    } else {
        return (seq_t)1;
    }
}
