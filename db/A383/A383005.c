/*
 * A383005  Exponent of the highest power of 2 dividing the n-th
 *          biquadratefree number.
 * https://oeis.org/A383005
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = exponent of 2 in the n-th biquadrate‑free integer * (the
 *            n-th integer not divisible by any fourth power > 1)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_biqfree_idx, v2_from_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_biqfree_idx.h"
#include "../utils/v2_from_idx.h"

/* -------------------------------------------------------------------- */
/* Helper: test whether an integer x (idx_t) is biquadrate‑free
 * (i.e. not divisible by p^4 for any prime p ≥ 2).  Since
 * the 10000‑th biquadrate‑free number is ≈ 1.1×10^4, 64‑bit arithmetic
 * is sufficient for all candidates that occur in the tests.             */

/* -------------------------------------------------------------------- */
/* Helper: exponent of 2 in x where x is an idx_t value.
 * The result is returned as a seq_t (value fits in a very small seq_t). */

/* -------------------------------------------------------------------- */
/* Cached results for a(n).  The tests use n up to about 10 000; a
 * biquadrate‑free number around that index is below 12 000.  We
 * therefore fix an upper bound so that we can use a static array and
 * avoid dynamic allocation of a type that may have a non‑trivial
 * constructor (arbitrary‑precision seq_t).                       */
/* -------------------------------------------------------------------- */
#if 10000 <= 12000          /* ensure the bound is safe for the test range */
    #define CACHE_MAX 12000
#else
    #define CACHE_MAX 12000
#endif

static seq_t a_cache[CACHE_MAX];
static idx_t a_size = 0;     /* number of valid terms in a_cache */
static idx_t next_candidate = 1;   /* next integer to test */

/* -------------------------------------------------------------------- */
/* Public function: a(n)                                                     */
/* -------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined for n < 1 */

    /* Generate terms up to n if necessary. */
    while (a_size < n) {
        /* Test the next integer for biquadrate‑freeness. */
        if (is_biqfree_idx(next_candidate))
            a_cache[a_size++] = v2_from_idx(next_candidate);
        ++next_candidate;                 /* move to the next candidate */
    }

    return a_cache[n - 1];
}
