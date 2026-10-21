/*
 * A257500  Numbers that cannot be expressed as the sum of distinct odd
 *          composite numbers.
 * https://oeis.org/A257500
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 31, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   build_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence of numbers that cannot be expressed as a sum of
   distinct odd composite numbers.
   For n ≥ 1, a(n) returns the n‑th term of the sequence. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/build_cache.h"

/* --------------------------------------------------------------------------
   1.  Compute the list of non‑representable numbers once.
   2.  All required numbers are ≤ 53, so a bound of 10000 is safely
       sufficient for the entire finite sequence.
   3.  The algorithm builds a DP bitset of representable sums using
       all odd composite numbers ≤ 10000.
   -------------------------------------------------------------------------- */

/* --------------------------------------------------------------------------
   Public entry point required by the build system.
   -------------------------------------------------------------- */
seq_t a(idx_t n)
{
    static bool initialized = false;
    static seq_t *nonrep = NULL;
    static size_t nonrep_len = 0;

    if (!initialized) {
        build_cache(&nonrep, &nonrep_len);
        initialized = true;
    }

    if (n == 0) return 0;            /* index zero not defined */
    if (n - 1 < nonrep_len)
        return nonrep[n - 1];
    else
        return 0;                    /* out of range; sequence is finite */
}
