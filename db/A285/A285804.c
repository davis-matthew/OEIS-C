/*
 * A285804  Composite numbers of the form 12*k+5 or 12*k+7 for some k.
 * https://oeis.org/A285804
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th composite number congruent to 5 or 7 modulo 12
 * Range:     u8 n <= 15, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Validated: partial, pass, 49 terms through n = 49 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

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

/* Return the n-th composite number of the form 12*k+5 or 12*k+7.
   Indices start at 1 (a(1) = 55).  For a given build the function will
   produce exact results for all terms that fit in the seq_t type. */
seq_t a(idx_t n)
{
    idx_t count = 0;          /* how many composite numbers found so far */
    idx_t k     = 0;          /* current multiplier of 12 */
    seq_t max_val = (seq_t)(-1);   /* maximum value representable in seq_t */

    while (1) {
        /* Candidate 12*k + 5 */
        idx_t cand = 12 * k + 5;
        if (cand <= (idx_t)max_val) {
            seq_t v = (seq_t)cand;
            if (!isPrimeGeneric(v)) {
                ++count;
                if (count == n) return v;
            }
        }

        /* Candidate 12*k + 7 */
        cand = 12 * k + 7;
        if (cand <= (idx_t)max_val) {
            seq_t v = (seq_t)cand;
            if (!isPrimeGeneric(v)) {
                ++count;
                if (count == n) return v;
            }
        }

        /* If both candidates exceed the representable range, we are
           unable to produce further terms; break to avoid infinite loop. */
        if (12 * k + 5 > (idx_t)max_val && 12 * k + 7 > (idx_t)max_val) {
            return 0;
        }
        ++k;
    }
}
