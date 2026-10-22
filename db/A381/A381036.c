/*
 * A381036  a(n) = 1 if there is a digit > 1 in the primorial base expansion
 *          of n, and the corresponding radix primes of all such digits are
 *          also prime factors of n, otherwise 0.
 * https://oeis.org/A381036
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if there is a digit > 1 in the primorial‐base expansion
 *            of n and each such digit's radix prime divides n; otherwise a(n)
 *            = 0
 * Range:     u8 n <= 9672, u16 n <= 9672, u32 n <= 9672, u64 n <= 9672,
 *            u128 n <= 9672, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

/* Return 1 if the condition holds, else 0. */
seq_t a(idx_t n)
{
    bool has_gt1 = false;          /* whether a digit > 1 was found   */
    idx_t rem = n;                 /* remainder while dividing       */

    for (idx_t p = 2; rem > 0; ++p) /* iterate over all primes          */
    {
        if (!is_prime(p))
            continue;              /* skip non‑primes                 */

        idx_t digit = rem % p;     /* current digit in radix p       */
        if (digit > 1)
        {
            has_gt1 = true;        /* at least one large digit       */
            if (n % p != 0)        /* radix prime must divide n      */
                return (seq_t)0;
        }
        rem /= p;                  /* move to next prime             */
    }

    return has_gt1 ? (seq_t)1 : (seq_t)0;
}
