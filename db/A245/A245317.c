/*
 * A245317  Concatenate n-th composite integer with n.
 * https://oeis.org/A245317
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = concat_parts(c_n, n) where c_n is the n-th composite
 *            integer
 * Range:     u8 n <= 9, u16 n <= 99, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   append_candidate, concat_parts, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

#include "../utils/is_prime_small.h"
#include "../utils/append_candidate.h"
#include "../utils/concat_parts.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined for n<1, but return 0 for safety */

    /* static cache of composites */
    static seq_t *comp_arr = NULL;   /* array of composites (seq_t) */
    static size_t comp_len = 0;      /* number of composites cached */
    static size_t comp_cap = 0;      /* allocated capacity */
    static idx_t next = 4;           /* next integer to test for compositeness */

    /* Expand cache until we have at least n composites */
    while (comp_len < (size_t)n)
    {
        /* test if 'next' is composite */
        if (!is_prime_small((uint64_t)next))
        {
            /* add to cache */
            append_candidate(&comp_arr, &comp_len, &comp_cap, (seq_t)next);
        }
        ++next;
    }

    /* Return concatenation of nth composite with n */
    return concat_parts((idx_t)comp_arr[n - 1], n);
}
