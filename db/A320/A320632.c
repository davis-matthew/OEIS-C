/*
 * A320632  Numbers k such that there exists a pair of factorizations of k
 *          into factors > 1 where no factor of one divides any factor of the
 *          other.
 * https://oeis.org/A320632
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 9439, u32 n <= 9439, u64 n <= 9439,
 *            u128 n <= 9439, big any n
 * Helpers:   factor_stats
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of A320632 by searching for numbers k such that
   sigma_0(k)-2 > (bigomega(k)-1)*omega(k),
   which is the conjectured characterization of the sequence. */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/factor_stats.h"

/* ----- static state for memoized sequence ----- */
static seq_t *seq_cache     = NULL;   /* array of computed terms */
static size_t seq_len       = 0;      /* number of values stored   */
static size_t seq_cap       = 0;      /* capacity of seq_cache     */
static uint64_t last_k       = 0;     /* highest k examined so far */

/* ----- sequence function ----- */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined for n==0 */

    size_t target = (size_t)n;  /* number of terms required */

    /* expand cache until it holds at least 'target' terms */
    while (seq_len < target) {
        /* next integer to test */
        uint64_t k = last_k + 1;
        last_k = k;

        uint64_t tau, bigomega, omega;
        factor_stats(k, &tau, &bigomega, &omega);

        /* use signed 128‑bit arithmetic to avoid underflow */
        __int128 left  = (__int128)tau - 2;
        __int128 right = (__int128)bigomega - 1;
        right *= (__int128)omega;

        if (left > right) {
            /* ensure capacity for the new element */
            if (seq_len == seq_cap) {
                size_t new_cap = seq_cap ? seq_cap * 2 : 4;
                seq_t *new_buf = (seq_t *)realloc(seq_cache,
                                                  new_cap * sizeof(seq_t));
                if (!new_buf) {
                    free(seq_cache);
                    seq_cache = NULL;
                    seq_cap = seq_len = 0;
                    return (seq_t)0;
                }
                seq_cache = new_buf;
                seq_cap   = new_cap;
            }
            seq_cache[seq_len++] = (seq_t)k;
        }
    }

    return seq_cache[target - 1];
}
