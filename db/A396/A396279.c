/*
 * A396279  Integers k such that p - k = floor(log_2(k)), where p is the
 *          smallest prime strictly greater than k.
 * https://oeis.org/A396279
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   floor_log2_uint64, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for integers k such that the difference to the
   smallest prime strictly greater than k equals floor(log₂(k))
   (OEIS A396279).  */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/floor_log2_uint64.h"
#include "../utils/is_prime.h"

/* Return the smallest prime strictly greater than start. */
static uint64_t next_prime(uint64_t start)
{
    for (uint64_t x = start + 1;; ++x)
        if (is_prime(x))
            return x;
}

/* seq_t a(idx_t n): return the n‑th term of the sequence
   (a(1) = 2).  The function works for any seq_t type
   (integer or arbitrary‑precision class). */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;              /* undefined for n == 0 */

    idx_t found = 0;
    uint64_t k = 2;                    /* first candidate */

    while (1) {
        uint64_t p   = next_prime(k);        /* smallest prime > k   */
        uint64_t gap = p - k;                /* prime gap             */
        uint64_t l2  = floor_log2_uint64(k); /* floor(log2(k))        */

        if (gap == l2) {                     /* condition met */
            ++found;
            if (found == n)
                return (seq_t)k;              /* cast to seq_t        */
        }
        ++k;                                 /* next candidate       */
    }
}
