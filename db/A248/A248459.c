/*
 * A248459  Number of length n+2 0..6 arrays with no three consecutive terms
 *          having the sum of any two elements equal to twice the third.
 * https://oeis.org/A248459
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 4, u32 n <= 10, u64 n <= 22, u128 n <= 47,
 *            big any n
 * Helpers:   forbidden, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A248459: Number of length n+2 0..6 arrays with no three consecutive
   terms forming an arithmetic progression.
   The DP keeps a 7×7 table of counts for sequences of the current length.
   At each step we append a new value v if the triple (a0,a1,v) is not
   arithmetic (i.e. a0 + a1 != 2*v && a0 + v != 2*a1 && a1 + v != 2*a0).
   a(n) is the total number after n transitions (starting from length‑2). */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"
#include "../utils/forbidden.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                               /* undefined for n=0 */

    /* static working state */
    static seq_t *seq = NULL;            /* cache of a(k) for k>=1 */
    static size_t seq_cap = 0;           /* allocated capacity (>=n+1) */
    static idx_t seq_len = 0;            /* largest n already computed   */
    static seq_t dp[7][7];               /* counts for current length    */
    static seq_t ndp[7][7];              /* temporary table for next step*/
    static bool initialised = false;     /* one‑time initialization flag */

    if (!initialised) {
        /* initial dp: all 7×7 pairs are possible for length 2 */
        for (int i = 0; i < 7; ++i)
            for (int j = 0; j < 7; ++j)
                dp[i][j] = (seq_t)1;

        seq_cap = 16;
        seq = seq_alloc(seq_cap);
        if (!seq) return 0;                     /* allocation failure */
        seq_len = 0;
        initialised = true;
    }

    /* enlarge cache if necessary */
    if ((size_t)n + 1 > seq_cap) {
        size_t new_cap = seq_cap ? seq_cap : 1;
        while ((size_t)n + 1 > new_cap) new_cap <<= 1;
        seq_t *tmp = seq_realloc(seq, seq_cap, new_cap);
        if (!tmp) return 0;                     /* bad alloc */
        seq = tmp;
        seq_cap = new_cap;
    }

    /* return cached value if already known */
    if (n <= seq_len) return seq[n];

    /* perform transitions until seq_len reaches n */
    for (; seq_len < n; ++seq_len) {
        /* zero ndp */
        for (int i = 0; i < 7; ++i)
            for (int j = 0; j < 7; ++j)
                ndp[i][j] = (seq_t)0;

        /* extend each existing pair by a new value v */
        for (int a0 = 0; a0 < 7; ++a0) {
            for (int a1 = 0; a1 < 7; ++a1) {
                seq_t cur = dp[a0][a1];
                if (cur == (seq_t)0) continue;
                for (int v = 0; v < 7; ++v) {
                    if (!forbidden(a0, a1, v))
                        ndp[a1][v] += cur;
                }
            }
        }

        /* sum all sequences of new length (seq_len+3) => a(seq_len+1) */
        seq_t total = (seq_t)0;
        for (int i = 0; i < 7; ++i)
            for (int j = 0; j < 7; ++j)
                total += ndp[i][j];

        /* copy ndp back to dp for next iteration */
        for (int i = 0; i < 7; ++i)
            for (int j = 0; j < 7; ++j)
                dp[i][j] = ndp[i][j];

        /* store result in cache */
        seq[seq_len + 1] = total;
    }

    return seq[n];
}
