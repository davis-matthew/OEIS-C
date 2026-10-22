/*
 * A373236  Relative of Hofstadter Q-sequence: a(n) = max(0, n+3201) for n <=
 *          0; a(n) = a(n-a(n-1)) + a(n-a(n-2)) + a(n-a(n-3)) for n > 0.
 * https://oeis.org/A373236
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = a(n-a(n-1)) + a(n-a(n-2)) + a(n-a(n-3)) (n > 0)
 * Range:     u8 n <= 1, u16 n <= 3725, u32 n <= 3725, u64 n <= 3725,
 *            u128 n <= 3725, big any n
 * Helpers:   _base_nonpos
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*           a(n) = max(0, n+3201)                           (n ≤ 0) */
/* a(0) = 3201. The function handles indices n ≥ 1 and lazily memoizes */
/* results so that subsequent calls are O(1).  Negative indices are   */
/* handled by the closed form.                                      */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/_base_nonpos.h"

/* cached sequence values for n ≥ 0.  memo[0] holds a(0).  */
static seq_t *memo = NULL;
static idx_t memo_size = 0;   /* current allocation size */

/* fetch a(k) where k may be negative.  For k ≥ 1 it returns memo[k],
   otherwise it returns the value defined for non‑positive indices. */
static inline seq_t
_get_a(int64_t k)
{
    if (k >= 1)
        return memo[k];
    else
        return _base_nonpos(k);
}

/* public routine: compute a(n) for n ≥ 1 */
seq_t
a(idx_t n)
{
    if (n == (idx_t)0)
        return _base_nonpos(0);

    /* grow cache if necessary */
    if (n >= memo_size) {
        idx_t new_size = n + 1;
        if (!memo) {
            memo = (seq_t *)malloc((size_t)new_size * sizeof(seq_t));
            if (!memo) return (seq_t)0;   /* allocation failure */
            memo[0] = _base_nonpos(0);
        } else {
            seq_t *tmp = (seq_t *)realloc(memo, (size_t)new_size * sizeof(seq_t));
            if (!tmp) return (seq_t)0;
            memo = tmp;
        }

        /* compute missing entries up to n */
        for (idx_t i = memo_size; i <= n; ++i) {
            int64_t i_s   = (int64_t)i;
            int64_t a1    = (int64_t)_get_a(i_s - 1);
            int64_t a2    = (int64_t)_get_a(i_s - 2);
            int64_t a3    = (int64_t)_get_a(i_s - 3);

            int64_t idx1 = i_s - a1;
            int64_t idx2 = i_s - a2;
            int64_t idx3 = i_s - a3;

            seq_t v1 = _get_a(idx1);
            seq_t v2 = _get_a(idx2);
            seq_t v3 = _get_a(idx3);

            memo[i] = v1 + v2 + v3;
        }
        memo_size = new_size;
    }
    return memo[n];
}
