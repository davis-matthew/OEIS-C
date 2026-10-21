/*
 * A276055  Number of palindromic compositions of n with parts in
 *          {1,2,4,6,8,10,...}.
 * https://oeis.org/A276055
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-2) + 2*a(n-4) - a(n-6) for n ≥ 6 with initial values
 *            a(0)=1, a(1)=1, a(2)=2, a(3)=1, a(4)=4, a(5)=2 (from the
 *            generating function g(z) = (1+z+z²‑z⁵)/(1‑z²‑2z⁴+z⁶))
 * Range:     u8 n <= 19, u16 n <= 37, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* compute a(n) – number of palindromic compositions of n with parts
   in {1,2,4,6,8,10,…}.  The function is memoised, works for any
   order of calls, and respects the defined types seq_t and idx_t. */

seq_t a(idx_t n)
{
    /* fast path for the first six terms – no allocation required */
    if (n <= 5) {
        static const seq_t first6[6] = {1, 1, 2, 1, 4, 2};
        return first6[(size_t)n];
    }

    /* static state for memoisation */
    static seq_t *dp   = NULL;   /* array of previously computed terms */
    static size_t cap  = 0;      /* allocated capacity */
    static idx_t   last = (idx_t)-1; /* highest index stored, -1 = not initialised */

    /* ensure array is at least big enough for n+1 terms */
    size_t need = (size_t)(n + 1);
    if (need > cap) {
        size_t new_cap = (need > 6 ? need : 6);
        if (dp == NULL) {
            dp = (seq_t*)malloc(new_cap * sizeof(seq_t));
            if (dp == NULL) exit(1);          /* allocation failed */
            /* initialise first six values */
            dp[0] = 1;
            dp[1] = 1;
            dp[2] = 2;
            dp[3] = 1;
            dp[4] = 4;
            dp[5] = 2;
            last  = 5;
        } else {
            seq_t *tmp = (seq_t*)realloc(dp, new_cap * sizeof(seq_t));
            if (tmp == NULL) exit(1);          /* allocation failed */
            dp   = tmp;
        }
        cap = new_cap;
    }

    /* compute any missing terms up to n */
    idx_t start = (last == (idx_t)-1 ? 0 : last + 1);
    for (idx_t i = start; i <= n; ++i) {
        if (i <= 5) {
            /* already initialised – nothing to do */
            if (i > last) last = i;
            continue;
        }
        /* a(i) = a(i-2) + 2*a(i-4) - a(i-6) */
        seq_t term = dp[i - 2];
        term   += dp[i - 4] << 1;   /* 2 * dp[i-4] */
        term   -= dp[i - 6];
        dp[i]   = term;
        last    = i;
    }

    return dp[n];
}
