/*
 * A358341  Expansion of e.g.f. (exp(x)-1)*(exp(x)-x)*(exp(x)-x^2/2).
 * https://oeis.org/A358341
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^n - 2^n - n*(2^(n-1)-1) - binom(n,2)*(2^(n-2)-1) +
 *            3*binom(n,3) (with a(0)=0, a(3)=7)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *pow2_cache = NULL;
static seq_t *pow3_cache = NULL;
static idx_t pow2_len = 0;
static idx_t pow3_len = 0;

/* Grow the 2‑power cache to include index n. */
static void ensure_pow2(idx_t n)
{
    if (n + 1 <= pow2_len)
        return;
    idx_t newlen = n + 1;
    seq_t *newcache = (seq_t *)realloc(pow2_cache, newlen * sizeof(seq_t));
    if (!newcache)
        exit(1);
    for (idx_t i = pow2_len; i < newlen; ++i) {
        if (i == 0)
            newcache[0] = (seq_t)1;
        else
            newcache[i] = newcache[i - 1] * (seq_t)2;
    }
    pow2_cache = newcache;
    pow2_len = newlen;
}

/* Grow the 3‑power cache to include index n. */
static void ensure_pow3(idx_t n)
{
    if (n + 1 <= pow3_len)
        return;
    idx_t newlen = n + 1;
    seq_t *newcache = (seq_t *)realloc(pow3_cache, newlen * sizeof(seq_t));
    if (!newcache)
        exit(1);
    for (idx_t i = pow3_len; i < newlen; ++i) {
        if (i == 0)
            newcache[0] = (seq_t)1;
        else
            newcache[i] = newcache[i - 1] * (seq_t)3;
    }
    pow3_cache = newcache;
    pow3_len = newlen;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* Ensure powers up to n are available. */
    if (pow2_len <= n)
        ensure_pow2(n);
    if (pow3_len <= n)
        ensure_pow3(n);

    seq_t pow3n   = pow3_cache[n];
    seq_t pow2n   = pow2_cache[n];
    seq_t res     = pow3n - pow2n;

    seq_t pow2n1  = pow2_cache[n - 1];
    res -= (seq_t)n * (pow2n1 - (seq_t)1);

    if (n >= 2) {
        seq_t pow2n2 = pow2_cache[n - 2];
        seq_t bin2   = ((seq_t)n * ((seq_t)n - (seq_t)1)) / (seq_t)2;
        res -= bin2 * (pow2n2 - (seq_t)1);
    }

    if (n > 3) {                 /* the term 3*binom(n,3) is omitted for n==3 */
        seq_t bin3   = ((seq_t)n * ((seq_t)n - (seq_t)1) * ((seq_t)n - (seq_t)2)) / (seq_t)6;
        res += (seq_t)3 * bin3;
    }

    return res;
}
