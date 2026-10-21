/*
 * A269882  Number of n X 2 0..5 arrays with some element plus some
 *          horizontally, diagonally, antidiagonally or vertically adjacent
 *          neighbor totalling five exactly once.
 * https://oeis.org/A269882
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 30*a(n‑1) – 197*a(n‑2) – 420*a(n‑3) – 196*a(n‑4); a(1)=6,
 *            a(2)=432, a(3)=11280, a(4)=250992, a(5)=5125008
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* static cache for memoised terms */
static seq_t *cache   = NULL;      /* array indexed 0..n (0 unused)      */
static size_t cachecap = 0;        /* allocated size (count of elements) */
static idx_t  cached   = 0;        /* highest index for which a(i) is stored */

/* Ensure the cache can hold indices up to and including n */
static void ensure_capacity(idx_t n)
{
    size_t need = (size_t)(n + 1);          /* count ∈ [0..n] inclusive      */
    if (need <= cachecap)
        return;
    seq_t *p = (seq_t *)realloc(cache, need * sizeof(seq_t));
    if (!p) abort();                       /* allocation failed */
    cache = p;
    if (cachecap < need) {
        /* zero new entries; index 0 remains unused            */
        memset(cache + cachecap, 0, (need - cachecap) * sizeof(seq_t));
    }
    cachecap = need;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                   /* offset is 1; unused 0th term */

    /* initialise the first five values if not already done */
    if (cached < 5) {
        /* make sure the cache can hold indices 1..5 */
        ensure_capacity(5);
        cache[1] = (seq_t)6;
        cache[2] = (seq_t)432;
        cache[3] = (seq_t)11280;
        cache[4] = (seq_t)250992;
        cache[5] = (seq_t)5125008;
        cached   = 5;
    }

    /* grow cache if needed for the requested index */
    if (n > cached)
        ensure_capacity(n);

    if (n <= cached)
        return cache[n];

    /* compute missing terms up to n */
    for (idx_t i = cached + 1; i <= n; ++i) {
        seq_t term = cache[i - 1] * (seq_t)30;
        term -= cache[i - 2] * (seq_t)197;
        term -= cache[i - 3] * (seq_t)420;
        term -= cache[i - 4] * (seq_t)196;
        cache[i] = term;
    }
    cached = n;
    return cache[n];
}
