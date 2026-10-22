/*
 * A373859  Smallest prime obtained by appending one or more 9's to n, -1 if
 *          no such prime exists.
 * https://oeis.org/A373859
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, sign
 * Formula:   not stated
 * Range:     i8 n <= 3, i16 n <= 10, i32 n <= 10, i64 n <= 30, i128 n <= 30,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for the smallest prime of the form n*10^k + (10^k-1).  
   If n ≡ 0 (mod 3) every candidate is a multiple of 3, so return -1. */
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* n divisible by 3 ⇒ all (n*10^k + 10^k - 1) are divisible by 3 */
    if ((n % 3) == 0)
        return -1;

    /* First candidate: k = 1.  10^1 == 10. */
    seq_t pow10_val = 10;                     /* 10^k */
    seq_t candidate  = (seq_t)n * pow10_val + (pow10_val - 1);  /* k=1 */

    if (is_prime_big(candidate))
        return candidate;

    /* Search for k = 2, 3, … up to a sensible bound. */
    for (idx_t k = 2; k <= 200; ++k)
    {
        /* Compute next 10^k, detect 64‑bit overflow if seq_t is a native int. */
        seq_t nextPow10 = pow10_val * 10;
        if (nextPow10 / 10 != pow10_val)   /* overflow detected */
            break;

        pow10_val = nextPow10;
        candidate = (seq_t)n * pow10_val + (pow10_val - 1);

        if (is_prime_big(candidate))
            return candidate;
    }

    /* No prime found within the search limits. */
    return -1;
}
