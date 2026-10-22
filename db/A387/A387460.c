/*
 * A387460  Composite cyclic numbers (subset of A050384) that are of the form
 *          4n+1.
 * https://oeis.org/A387460
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   A387460 – composite cyclic numbers of the form 4k+1
 * Range:     u8 n <= 16, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   calc_phi, gcd, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  This function returns the n-th element of the sequence.  It uses
    a simple incremental search starting from the smallest number,
    retaining a static cache so that calls with arbitrary n are fast.  */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/gcd.h"
#include "../utils/calc_phi.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined, return 0 for safety */

    /* Static cache of computed terms */
    static seq_t *cache = NULL;           /* array of seq_t values          */
    static size_t cache_len = 0;          /* number of terms stored        */
    static size_t cache_cap = 0;          /* allocated capacity of array    */
    static idx_t last_candidate = 1;      /* last tested number            */

    /* Grow the cache array if necessary */
    if (cache_len < n) {
        size_t needed = n;
        if (cache_cap < needed) {
            size_t new_cap = (cache_cap == 0) ? 64 : cache_cap * 2;
            while (new_cap < needed) new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) {
                /* allocation failed – in a real program you might abort */
                return 0;
            }
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Search for the next terms until we have n of them */
        while (cache_len < n) {
            /* Advance to the next integer of the form 4k+1 */
            last_candidate++;
            if (last_candidate % 4 != 1)
                continue;

            /* Convert to seq_t for operations that need it */
            seq_t num = (seq_t)last_candidate;

            /* Skip primes – we only want composite numbers */
            if (is_prime_big(num))
                continue;

            /* Compute Euler's totient */
            seq_t phi_val = calc_phi(num);

            /* Check cyclicity: gcd(num, phi_val) == 1 */
            if (gcd(num, phi_val) == (seq_t)1) {
                cache[cache_len++] = num;
            }
        }
    }

    /* Return the requested term (1‑based index) */
    return cache[n - 1];
}
