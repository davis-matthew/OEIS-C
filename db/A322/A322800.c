/*
 * A322800  Number of compositions (ordered partitions) of n into octagonal
 *          numbers (A000567).
 * https://oeis.org/A322800
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ordered compositions of n into octagonal
 *            numbers (A000567). The recurrence is a(0) = 1; a(n) = Σ_{oct ≤
 *            n} a(n - oct), where oct runs over all octagonal numbers
 *            k(3k-2), k ≥ 1
 * Range:     u8 n <= 30, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* ------------------------------------------------------------------ */
/* globals: memoised values and the list of octagonal numbers        */
/* ------------------------------------------------------------------ */

static seq_t *dp      = NULL;          /* a[0 .. dp_len-1]             */
static size_t dp_cap  = 0;             /* allocated size of dp array   */
static idx_t  dp_len  = 0;             /* number of elements stored    */

static idx_t *oct     = NULL;          /* array of octagonal numbers   */
static size_t oct_cap = 0;             /* allocated size of oct array  */
static idx_t  oct_len = 0;             /* number of values stored      */

/* ------------------------------------------------------------------ */
/* grow the capacity of a dynamic array by doubling its size          */
/* ------------------------------------------------------------------ */
static size_t grow_cap(size_t cap)
{
    return (cap == 0) ? 1 : cap << 1;
}

/* ------------------------------------------------------------------ */
/* ensure that oct[] contains every octagonal number ≤ limit         */
/* ------------------------------------------------------------------ */
static void
ensure_oct(idx_t limit)
{
    /* If we've never initialized oct[], start at k = 1; otherwise start
       after the last stored value.  */
    idx_t k = oct_len + 1;
    for (;;) {
        idx_t w = k * (3 * k - 2);     /* k(3k-2)  */
        if (w > limit) break;

        if (oct_len == oct_cap) {
            size_t new_cap = grow_cap(oct_cap);
            idx_t *tmp = (idx_t *)realloc(oct, new_cap * sizeof(idx_t));
            if (!tmp) abort();        /* out of memory */
            oct = tmp;
            oct_cap = new_cap;
        }
        oct[oct_len++] = w;
        ++k;
    }
}

/* ------------------------------------------------------------------ */
/* main function: returns a(n)                                       */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Initialise dp[0] = 1 on first call. */
    if (!dp) {
        dp = (seq_t *)malloc(sizeof(seq_t));
        if (!dp) abort();
        dp[0] = (seq_t)1;
        dp_len = 1;
        dp_cap = 1;
    }

    /* Grow dp array as needed. */
    if (n >= dp_len) {
        if ((size_t)n + 1 > dp_cap) {
            size_t new_cap = grow_cap(dp_cap);
            while (new_cap <= (size_t)n + 1) new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(dp, new_cap * sizeof(seq_t));
            if (!tmp) abort();
            dp = tmp;
            dp_cap = new_cap;
        }

        /* Ensure we have all octagonal numbers up to n. */
        ensure_oct(n);

        /* Compute a[i] for all i that are not yet computed. */
        for (idx_t i = dp_len; i <= n; ++i) {
            seq_t sum = (seq_t)0;
            for (idx_t j = 0; j < oct_len; ++j) {
                idx_t o = oct[j];
                if (o > i) break;
                sum += dp[i - o];
            }
            dp[i] = sum;
        }
        dp_len = n + 1;
    }

    return dp[n];
}
