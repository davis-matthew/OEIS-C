/*
 * A247152  Least composite which contains prime(n) as a substring.
 * https://oeis.org/A247152
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = min{ concat(1, prime(n)), concat(2, prime(n)),
 *            10*prime(n) }
 * Range:     u8 n <= 9, u16 n <= 935, u32 n <= 5865, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   concat_parts, get_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/concat_parts.h"
#include "../utils/is_prime_big.h"
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* Offset is 1: a(1) corresponds to n==1. For n==0, behaviour is undefined. */
    if (n == 0)
        return 0;

    /* Retrieve nth prime.  get_prime is defined in the utils library. */
    uint32_t p = get_prime(n);

    /* Candidate composite numbers:
       1. "1" concatenated with prime p  -> 1p
       2. "2" concatenated with prime p  -> 2p
       3. p followed by 0                -> 10*p          (always composite) */
    seq_t cand1 = concat_parts(1, p);          /* 1p  */
    seq_t cand2 = concat_parts(2, p);          /* 2p  */
    seq_t cand3 = (seq_t)p * 10;               /* 10p */

    /* Initial minimum is cand3 because it is guaranteed composite. */
    seq_t min = cand3;

    /* Helper lambda to update min if candidate is composite and smaller. */
    #define UPDATE_MIN(c)                      \
        if (!is_prime_big(c) && (c) < min) {   \
            min = c;                           \
        }

    UPDATE_MIN(cand1);
    UPDATE_MIN(cand2);
    /* cand3 is already composite, no need to test. */

    #undef UPDATE_MIN

    return min;
}
