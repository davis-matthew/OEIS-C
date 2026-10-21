/*
 * A278000  Number of n X 2 0..2 arrays with every element plus 1 mod 3 equal
 *          to some element at offset (-1,-1) (-1,0) (-1,1) (0,-1) (0,1)
 *          (1,-1) or (1,0), with upper left element zero.
 * https://oeis.org/A278000
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A278000 – Number of \( n \times 2\) 0..2 arrays with the
    “plus‑1 mod‑3” neighbour condition.  The sequence satisfies
    the linear recurrence

        a(n) = 6*a(n‑1) + 6*a(n‑2) + 3*a(n‑3) – 6*a(n‑4)

    with initial values
        a(1)=0 , a(2)=9 , a(3)=52 , a(4)=364.
*/

#include <stdlib.h>

/* --------------------------------------------------------------------- */
/*  Memoised computation of the recurrence, O(n) time for every call.   */
/* --------------------------------------------------------------------- */

static seq_t *cache = NULL;          /* array indexed from 1 to cached_len */
static idx_t  cached_len = 0;        /* highest index stored */

static void ensure_up_to(idx_t n)
{
    if (cached_len >= n) return;     /* already computed */

    idx_t prev_len = cached_len;     /* keep the old size to know where to start */

    if (prev_len == 0) {             /* first call – initialise base values */
        cached_len = (n > 4) ? n : 4;              /* we always keep first 4 */
        cache = (seq_t *)malloc((cached_len + 1) * sizeof(seq_t));
        if (!cache) abort();
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)9;
        cache[3] = (seq_t)52;
        cache[4] = (seq_t)364;
    } else {                         /* enlarge array */
        cached_len = n;
        cache = (seq_t *)realloc(cache, (cached_len + 1) * sizeof(seq_t));
        if (!cache) abort();
    }

    /* Compute missing terms only from (prev_len+1) up to cached_len */
    for (idx_t i = prev_len + 1; i <= cached_len; ++i) {
        if (i >= 5) {
            cache[i] = (seq_t)6 * cache[i-1] +
                       (seq_t)6 * cache[i-2] +
                       (seq_t)3 * cache[i-3] -
                       (seq_t)6 * cache[i-4];
        }
        /* i = 1..4 are already the initial values */
    }
}

/* ------------------------------------------------------------ */
/*  Public function required by the build.                      */
/* ------------------------------------------------------------ */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* defined only for n >= 1, but safe */
    ensure_up_to(n);
    return cache[n];
}
