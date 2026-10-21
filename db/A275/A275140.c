/*
 * A275140  Number of n X 6 0..2 arrays with no element equal to any value at
 *          offset (-2,-2) (-1,-2) or (0,-1) and new values introduced in
 *          order 0..2.
 * https://oeis.org/A275140
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + 10*a(n-2) + 4*a(n-3) - 4*a(n-4) for n > 6 with
 *            a(1)=16, a(2)=432, a(3)=1216, a(4)=6464, a(5)=32384, a(6)=165568
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 12, u64 n <= 25, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *seq_cache = NULL;   /* 1‑based storage */
static idx_t seq_cache_len = 0;   /* length of the allocated array */

/* Ensure cache can index up to n (inclusive). */
static void ensure_cache(idx_t n)
{
    if (seq_cache_len > n)
        return;
    idx_t newlen = n + 1;               /* indices 0..n */
    seq_t *tmp = (seq_t *)realloc(seq_cache, newlen * sizeof(seq_t));
    if (!tmp)                           /* allocation failed – terminate */
        abort();
    seq_cache = tmp;
    seq_cache_len = newlen;
}

/* Initialize the first six terms of the sequence. */
static void init_base(void)
{
    ensure_cache(6);   /* allocate indices 0..6 */
    seq_cache[1] = (seq_t)16;
    seq_cache[2] = (seq_t)432;
    seq_cache[3] = (seq_t)1216;
    seq_cache[4] = (seq_t)6464;
    seq_cache[5] = (seq_t)32384;
    seq_cache[6] = (seq_t)165568;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (!seq_cache)          /* first call – set up base values */
        init_base();

    if (n <= 6)
        return seq_cache[n];

    ensure_cache(n);         /* make sure the array is large enough */

    /* Compute terms lazily up to n. */
    for (idx_t i = 7; i <= n; ++i) {
        seq_t v =
            ((seq_t)3) * seq_cache[i-1] +
            ((seq_t)10) * seq_cache[i-2] +
            ((seq_t)4) * seq_cache[i-3] -
            ((seq_t)4) * seq_cache[i-4];
        seq_cache[i] = v;
    }
    return seq_cache[n];
}
