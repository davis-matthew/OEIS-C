/*
 * A282453  Decimal representation of the x-axis, from the left edge to the
 *          origin, of the n-th stage of growth of the two-dimensional
 *          cellular automaton defined by "Rule 475", based on the 5-celled
 *          von Neumann neighborhood.
 * https://oeis.org/A282453
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2k) = 2^(2k+1)-1; a(2k+1) = 2^(2k)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 126,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

static seq_t *pow_cache = NULL;
static size_t pow_cap = 0;
static size_t pow_len = 0;

static void ensure_pow(size_t exp)
{
    if (!pow_cache) {
        pow_cap = 16;
        pow_len = 1;
        pow_cache = seq_alloc(pow_cap);
        pow_cache[0] = (seq_t)1;
    }
    if (exp >= pow_len) {
        while (pow_len <= exp) {
            if (pow_len == pow_cap) {
                size_t old_cap = pow_cap;
                pow_cap <<= 1;
                pow_cache = seq_realloc(pow_cache, old_cap, pow_cap);
            }
            pow_cache[pow_len] = pow_cache[pow_len - 1] * ((seq_t)2);
            pow_len++;
        }
    }
}

seq_t a(idx_t n)
{
    /* Ensure powers up to n+1 are available */
    ensure_pow((size_t)(n + 1));
    if ((n & ((idx_t)1)) == 0) {          /* even n */
        return pow_cache[(size_t)(n + 1)] - ((seq_t)1);
    } else {                               /* odd n */
        return pow_cache[(size_t)(n - 1)];
    }
}
