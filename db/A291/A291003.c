/*
 * A291003  p-INVERT of (1,1,1,1,1,...), where p(S) = (1 - S)(1 - 2*S)(1 -
 *          3*S)(1 - 4*S).
 * https://oeis.org/A291003
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 11, u64 n <= 25, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = 10, 75, 490, 2956, 16944, … – p‑INVERT of (1,1,1,1,1,…)
   Recurrence (for n ≥ 5):
     a(n) = 14*a(n‑1) – 71*a(n‑2) + 154*a(n‑3) – 120*a(n‑4)   */

/* Compile with C11 or C++17.  Set seq_t and idx_t via the build system. */

#include <stdlib.h>

static seq_t *cache   = NULL;   /* array of computed terms          */
static size_t   cap    = 0;     /* allocated capacity (in terms)   */
static size_t   len    = 0;     /* number of terms already filled  */

seq_t a(idx_t n)
{
    /* Minimum initial values from the OEIS table. */
    static const seq_t init[5] = {10, 75, 490, 2956, 16944};

    /* Convert idx_t to size_t for array indexing (valid for all useful n). */
    size_t idx = (size_t)n;

    /* Initialise cache if first call. */
    if (cache == NULL) {
        cap = 5;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return 0;                /* out of memory – caller must not request such n */
        for (size_t i = 0; i < 5; ++i)
            cache[i] = init[i];
        len = 5;
    }

    if (idx < len)            /* already computed */
        return cache[idx];

    /* Grow cache to at least idx+1 terms. */
    if (cap <= idx) {
        size_t newcap = cap ? cap : 1;
        while (newcap <= idx) newcap <<= 1;        /* double until large enough */
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) return 0;                       /* allocation failed */
        cache = tmp;
        cap   = newcap;
    }

    /* Compute missing terms up to n using the recurrence. */
    for (size_t i = len; i <= idx; ++i) {
        seq_t a1 = cache[i - 1];
        seq_t a2 = cache[i - 2];
        seq_t a3 = cache[i - 3];
        seq_t a4 = cache[i - 4];

        /* Uses built‑in operators; works for both built‑in integers
           and arbitrary‑precision classes that support +, -, * with
           integer literals. */
        seq_t val = 14 * a1 - 71 * a2 + 154 * a3 - 120 * a4;
        cache[i] = val;
    }
    len = idx + 1;
    return cache[idx];
}
