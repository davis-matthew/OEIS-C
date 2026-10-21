/*
 * A289615  A246604 (Catalan(n)-n) with initial terms 1,0,0,2 changed to
 *          1,1,1,3.
 * https://oeis.org/A289615
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Catalan(n) - n, with a(0)=1, a(1)=1, a(2)=1, a(3)=3
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 17, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* compute Catalan numbers recursively and cache them */
#include <stdlib.h>

static seq_t *catalan_cache = NULL;
static idx_t catalan_len = 0;
static idx_t catalan_cap = 0;

static void ensure_catalan(idx_t n)
{
    /* ensure cache contains Catalan numbers up to index n */
    if (n < catalan_len)
        return;
    if (catalan_cache == NULL) {
        /* initial allocation */
        catalan_cap = 1;
        catalan_cache = (seq_t *)malloc(sizeof(seq_t) * catalan_cap);
        if (!catalan_cache)
            return; /* allocation failed – should not happen in tests */
        catalan_cache[0] = (seq_t)1;   /* C0 = 1 */
        catalan_len = 1;
    }
    if (n + 1 > catalan_cap) {
        /* grow capacity (double strategy) */
        idx_t new_cap = catalan_cap;
        while (new_cap <= n) {
            new_cap *= 2;
        }
        seq_t *tmp = (seq_t *)realloc(catalan_cache, sizeof(seq_t) * new_cap);
        if (!tmp)
            return; /* allocation failed – should not happen in tests */
        catalan_cache = tmp;
        catalan_cap = new_cap;
    }
    /* compute missing Catalan values */
    for (idx_t i = catalan_len; i <= n; ++i) {
        seq_t prev = catalan_cache[i - 1];
        /* C_i = C_{i-1} * 2*(2*(i-1)+1) / (i+1) */
        seq_t numerator = (seq_t)2 * (seq_t)(2 * (i - 1) + 1);
        seq_t denominator = (seq_t)(i + 1);
        seq_t val = prev * numerator / denominator;
        catalan_cache[i] = val;
    }
    catalan_len = n + 1;
}

seq_t a(idx_t n)
{
    if (n < 3)
        return (seq_t)1;
    if (n == 3)
        return (seq_t)3;
    ensure_catalan(n);
    return catalan_cache[n] - (seq_t)n;
}
