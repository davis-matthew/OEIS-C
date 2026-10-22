/*
 * A350535  Integers that cannot be expressed as x + y + z + x*y*z for x,y,z
 *          >= 1.
 * https://oeis.org/A350535
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   append_candidate, isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS A350535: integers that cannot be expressed as
   x + y + z + x*y*z for x,y,z >= 1.  The function computes the
   sequence by searching successive primes and testing representability. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "../utils/isPrimeGeneric.h"
#include "../utils/append_candidate.h"

/* Check whether a given integer p can be written as x + y + z + x*y*z
   with x,y,z >= 1.  Return true if such a representation exists. */
static bool is_representable(seq_t p)
{
    if (p < 4)   /* the minimal value of x + y + z + x*y*z is 4 */
        return false;

    /* Outer loop over x.  x*x <= p guarantees that a solution, if any,
       must have x <= sqrt(p). */
    for (idx_t x = 1L; ; ++x)
    {
        if ((seq_t)x * (seq_t)x > p)
            break;

        /* Inner loop over y.  For fixed x, as y increases the
           expression (p - x - y)/(1 + x*y) becomes smaller and
           eventually < 1; when that occurs further y will not work. */
        for (idx_t y = 1L; ; ++y)
        {
            seq_t denom = 1 + (seq_t)x * (seq_t)y;
            seq_t num   = p - (seq_t)x - (seq_t)y;

            if (num < denom)          /* z would be < 1 */
                break;

            if (num % denom == 0)     /* integer z */
            {
                seq_t z = num / denom;
                if (z >= 1)
                    return true;
            }
        }
    }
    return false;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Static cache for consecutive terms */
    static seq_t *cache      = NULL;
    static size_t cache_len  = 0;
    static size_t cache_cap  = 0;
    static seq_t last_checked = 1;   /* last integer examined */

    /* Initialise cache with the first element a(1)=1 */
    if (cache_len == 0)
    {
        append_candidate(&cache, &cache_len, &cache_cap, (seq_t)1);
        last_checked = 1;
    }

    /* Generate terms until we have at least n of them */
    while (cache_len < n)
    {
        /* Move to the next integer candidate */
        last_checked++;

        /* Only odd numbers (except 2) can be in the sequence,
           because even numbers > 2 are all representable. */
        if (last_checked > (seq_t)2 && (last_checked & 1ULL) == 0)
            continue;

        /* Skip non‑primes: all non‑prime integers, except 1,
           are representable. */
        if (!isPrimeGeneric(last_checked))
            continue;

        /* If the prime cannot be represented, it belongs to the
           sequence. */
        if (!is_representable(last_checked))
            append_candidate(&cache, &cache_len, &cache_cap, last_checked);
    }

    return cache[n - 1];
}
