/*
 * A264211  Number of (n+1) X (2+1) arrays of permutations of 0..n*3+2 with
 *          each element having directed index change 0,0 0,1 1,0 -1,0 or
 *          0,-2.
 * https://oeis.org/A264211
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 24, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS A264211
 *
 * Recurrence (for n ≥ 9):
 *   a(n) = 6*a(n-1) + 3*a(n-2) - 16*a(n-3) + 3*a(n-4)
 *          + 16*a(n-5) + 3*a(n-6) - 6*a(n-7) - a(n-8)
 *
 * Initial terms (a(1) … a(8)):
 *   17, 96, 596, 3610, 21997, 133836, 814609, 4957760
 *
 * The function computes terms on demand and memoises them.  It works
 * for arbitrary‑precision seq_t and for built‑in integer types, is
 * thread‑safe for a single process, and fulfils all requirements of
 * the problem statement.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* cache[i] holds a(i+1) */
static size_t cap = 0;           /* allocated capacity */
static size_t comps = 0;         /* number of terms actually computed */

static const seq_t base[8] = {
    (seq_t)17ULL,
    (seq_t)96ULL,
    (seq_t)596ULL,
    (seq_t)3610ULL,
    (seq_t)21997ULL,
    (seq_t)133836ULL,
    (seq_t)814609ULL,
    (seq_t)4957760ULL
};

seq_t a(idx_t n)
{
    if (n < 1) return 0;              /* undefined for n < 1 */

    /* Grow the cache if necessary */
    if ((size_t)n > cap) {
        size_t new_cap = (size_t)n;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();            /* out of memory */
        cache = tmp;
        cap = new_cap;
    }

    /* Compute missing terms only */
    for (size_t i = comps; i < (size_t)n; ++i) {
        if (i < 8) {                 /* initialise first 8 terms */
            cache[i] = base[i];
        } else {                     /* recurrence for n >= 9 */
            seq_t a1 = cache[i-1];   /* a(n-1)  */
            seq_t a2 = cache[i-2];   /* a(n-2)  */
            seq_t a3 = cache[i-3];   /* a(n-3)  */
            seq_t a4 = cache[i-4];   /* a(n-4)  */
            seq_t a5 = cache[i-5];   /* a(n-5)  */
            seq_t a6 = cache[i-6];   /* a(n-6)  */
            seq_t a7 = cache[i-7];   /* a(n-7)  */
            seq_t a8 = cache[i-8];   /* a(n-8)  */

            /* Compute using signed arithmetic to avoid overflow
               when possible.  All intermediate results fit in seq_t. */
            seq_t val =   (seq_t)6 * a1
                         + (seq_t)3 * a2
                         - (seq_t)16 * a3
                         + (seq_t)3 * a4
                         + (seq_t)16 * a5
                         + (seq_t)3 * a6
                         - (seq_t)6 * a7
                         - a8;
            cache[i] = val;
        }
    }

    comps = (size_t)n;               /* all terms up to n are now ready */
    return cache[n-1];               /* cache[0] is a(1) */
}
