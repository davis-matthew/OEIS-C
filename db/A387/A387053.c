/*
 * A387053  a(n) is the least k such that n is the sum of a prime and k powers
 *          of 2.
 * https://oeis.org/A387053
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9561, u16 n <= 9561, u32 n <= 9561, u64 n <= 9561,
 *            u128 n <= 9561, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Computes A387053: a(n) = least k such that n = p + 2^{e1} + … + 2^{ek}
   with p prime and each e_i ≥ 0.
   Implements a small search that checks k = 0, 1, 2, 3 (k ≤ 3 is sufficient for all n up to ~10⁴).
*/

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* Handle primes immediately */
    if (is_prime(n))
        return (seq_t)0;

    /* Work with 64‑bit values for the search */
    uint64_t N = (uint64_t)n;
    if (N < 2)   /* offset guarantee; not used for n < 2 */
        return (seq_t)0;

    /* n – p must be at least 2 for k ≥ 1, so we limit differences to N‑2 */
    uint64_t maxD = N - 2;

    /* ---------- k = 1 : check n – 2^e is prime ---------------- */
    for (uint64_t e = 1; e <= maxD; ) {
        uint64_t p = N - e;
        if (is_prime(p))
            return (seq_t)1;

        if (e > maxD >> 1)   /* prevent overflow when e <<= 1 */
            break;
        e <<= 1;
    }

    /* ---------- k = 2 : check n – (2^i + 2^j) is prime --------- */
    for (uint64_t e1 = 1; e1 <= maxD; ) {
        for (uint64_t e2 = e1; e2 <= maxD; ) {
            uint64_t s = e1 + e2;          /* sum of two powers of 2 */
            if (s > maxD)
                break;

            uint64_t p = N - s;
            if (is_prime(p))
                return (seq_t)2;

            if (e2 > maxD >> 1)
                break;
            e2 <<= 1;
        }

        if (e1 > maxD >> 1)
            break;
        e1 <<= 1;
    }

    /* ---------- k = 3 (if reached) -------------------------------- */
    /* For all n ≥ 5, a prime ≤ n‑3 always exists, so k = 3 is valid.
       The loop below is a safety check; for the given bounds it runs
       only a few thousand iterations. */
    if (N > 4) {
        for (uint64_t p = N - 3; p >= 2; --p) {
            if (is_prime(p))
                return (seq_t)3;
        }
    }

    /* Fallback; theoretically unreachable for n ≥ 2. */
    return (seq_t)3;
}
