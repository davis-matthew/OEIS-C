/*
 * A318982  a(n) = Sum_{d|n} Kronecker(-67, d).
 * https://oeis.org/A318982
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = Sum_{d|n} Kronecker(-67, d) (OEIS A318982)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   jacobi_symbol
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses the multiplicative property:
   For n = ∏ p^e,
     if p = 67        →  a(p^e) = 1
     if Kronecker(-67,p) = 1   →  a(p^e) = e + 1
     if Kronecker(-67,p) = -1  →  a(p^e) = (1 + (-1)^e)/2
*/

#include <stdint.h>
#include "../utils/jacobi_symbol.h"

/* Kronecker symbol (-67 | p) for an odd prime p (p ≠ 67) and p = 2. */
static int _kronecker_minus67(uint64_t p)
{
    /* Special case for p = 2: (-67|2) = -1 because -67 ≡ 5 (mod 8). */
    if (p == 2ULL) return -1;

    /* Compute -67 modulo p (in the range 0 .. p-1). */
    uint64_t rem = 67ULL % p;
    uint64_t a   = (p - rem) % p;   /* a ≡ -67 (mod p), 0 ≤ a < p */

    /* For p = 67, a == 0 giving symbol 0; but we handle this separately in a(). */
    return jacobi_symbol(a, p);      /* jacobi_symbol returns -1, 0, or 1 */
}

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;

    uint64_t num   = (uint64_t)n;
    seq_t    result = 1;

    /* Trial division factorisation.  p*p <= num uses the current num, which is
       valid because n ≤ 10000 in the test set.  The loop terminates when the
       remaining num is either 1 or a prime > sqrt(original n). */
    for (uint64_t p = 2; p * p <= num; ++p) {
        if (num % p != 0) continue;

        uint64_t exp = 0;
        while (num % p == 0) {
            num /= p;
            ++exp;
        }

        seq_t local;
        if (p == 67ULL) {
            local = 1;               /* a(67^e) = 1 */
        } else {
            int kr = _kronecker_minus67(p);
            if (kr == 1) {
                local = (seq_t)(exp + 1);          /* e + 1 */
            } else { /* kr == -1 */
                local = ((exp & 1) == 0) ? 1 : 0;  /* (1+(-1)^e)/2 */
            }
        }

        result *= local;
        if (result == 0) return (seq_t)0; /* early exit for zero factor */
    }

    /* If any prime factor > sqrt(original n) remains. */
    if (num > 1) {
        uint64_t p = num;
        seq_t local;
        if (p == 67ULL) {
            local = 1;
        } else {
            int kr = _kronecker_minus67(p);
            if (kr == 1) {
                local = 2;  /* exponent = 1 → e+1 = 2 */
            } else {
                local = 0;  /* exponent = 1, kr = -1 → 0 */
            }
        }
        result *= local;
    }

    return result;
}
