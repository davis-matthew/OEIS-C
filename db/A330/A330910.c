/*
 * A330910  a(n-5) is the number of nonempty subsets of {1,2,...,n} such that
 *          the difference of successive elements is at least 5.
 * https://oeis.org/A330910
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 32, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A330910 – number of nonempty subsets of {1,…,n+5} with differences ≥ 5
 *  (subsets must contain at least two elements).
 *
 *  Let f(N) be the number of subsets (including ∅) of {1,…,N} with
 *  successive elements at distance ≥ 5. Then:
 *
 *      f(N) = N+1                     for N ≤ 4
 *      f(N) = f(N-1) + f(N-5)         for N ≥ 5
 *
 *  The desired term a(n) counts only subsets of size ≥ 2, so
 *
 *      a(n) = f(n+5) – 1 – (n+5)
 *
 *  The first few values (offset 0) are
 *      0,1,3,6,10,15,22,32,46,65,…
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *f_cache = NULL;   /* f(N) values   */
static idx_t  f_last = 0;       /* highest N computed */

static void
ensure_f_cache(idx_t N)
{
    if (f_cache == NULL) {
        /* allocate for 0..N */
        f_cache = (seq_t *)malloc((N + 1) * sizeof(seq_t));
        if (!f_cache) { /* allocation failure – just return 0 */
            return;
        }
        f_last = N;
        /* compute values */
        for (idx_t k = 0; k <= N; ++k) {
            if (k <= 4)
                f_cache[k] = (seq_t)(k + 1);
            else
                f_cache[k] = f_cache[k-1] + f_cache[k-5];
        }
        return;
    }

    if (N <= f_last) {
        /* already computed up to N */
        return;
    }

    /* enlarge the cache */
    seq_t *tmp = (seq_t *)realloc(f_cache, (N + 1) * sizeof(seq_t));
    if (!tmp) {
        /* allocation failure – just leave old cache */
        return;
    }
    f_cache = tmp;
    /* compute new values */
    for (idx_t k = f_last + 1; k <= N; ++k) {
        if (k <= 4)
            f_cache[k] = (seq_t)(k + 1);
        else
            f_cache[k] = f_cache[k-1] + f_cache[k-5];
    }
    f_last = N;
}

seq_t
a(idx_t n)
{
    /* n is the index of the sequence (offset 0) */
    idx_t N = n + 5;          /* convert to set size 1..N */
    ensure_f_cache(N);
    if (N > f_last || f_cache == NULL) {
        /* safety: if cache failed to allocate, return 0 */
        return (seq_t)0;
    }
    seq_t fN = f_cache[N];
    seq_t result = fN - (seq_t)1 - (seq_t)(n + 5);
    return result;
}
