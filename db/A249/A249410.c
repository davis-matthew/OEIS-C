/*
 * A249410  Primes p such that sigma(p-1) is odd.
 * https://oeis.org/A249410
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 69, u32 n <= 7869, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 61 terms through n = 8871 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements OEIS A249410: primes p such that sigma(p‑1) is odd.
   Since sigma(m) is odd iff m is a square or twice a square,
   p can only be of the forms k²+1 or 2·k²+1.  The two sequences
   k → k²+1 and k → 2·k²+1 are strictly increasing, so we merge
   them to produce primes in sorted order. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <limits.h>
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"

/* -----------------  Generic primality test ------------------------ */
static bool isPrimeGeneric(seq_t n)
{
    /* all seq_t are unsigned; use the smallest type that fits */
    if ((seq_t)(ULLONG_MAX) >= n) {
        return is_prime_u64((uint64_t)n);
    }
#if defined(__SIZEOF_INT128__)
    return is_prime_u128((__uint128_t)n);
#else
    /* fallback: if seq_t > 64‑bit but no 128‑bit support,
       order the test as simple trial division up to sqrt */
    seq_t limit = 0;
    for (seq_t i = 2; i*i <= n; ++i) {
        if (n % i == 0) return false;
    }
    return true;
#endif
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* undefined, but safe */
    /* indices running along the two quadratic sequences */
    idx_t k1 = 1, k2 = 1;
    /* current next prime from each sequence */
    seq_t cur1 = 0, cur2 = 0;
    /* count of primes produced so far */
    idx_t cnt = 0;
    seq_t result = 0;

    while (cnt < n) {
        /* find the next prime in sequence k1²+1 */
        while (1) {
            seq_t val1 = (seq_t)k1 * k1 + 1;
            if (isPrimeGeneric(val1)) {   /* prime found */
                cur1 = val1;
                break;
            }
            k1++;                         /* skip composite */
        }
        /* find the next prime in sequence 2·k2²+1 */
        while (1) {
            seq_t val2 = (seq_t)2 * k2 * k2 + 1;
            if (isPrimeGeneric(val2)) {   /* prime found */
                cur2 = val2;
                break;
            }
            k2++;                         /* skip composite */
        }
        /* choose the smaller of the two candidates */
        if (cur1 <= cur2) {
            result = cur1;
            k1++;                         /* advance after use */
        } else {
            result = cur2;
            k2++;                         /* advance after use */
        }

        cnt++;
    }

    return result;
}
