/*
 * A391890  a(n) = Sum_{k=0..n} (k+1) * binomial(2*k+1,2*n-2*k+1).
 * https://oeis.org/A391890
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0..n} (k+1)·C(2k+1,2n−2k+1) Recurrence: a(0)=1,
 *            a(1)=6, a(2)=17, a(3)=58, a(4)=188, a(5)=570, a(6)=1715,
 *            a(7)=5074, a(n) = 4·a(n‑1) −2·a(n‑2) −11·a(n‑4) −2·a(n‑6)
 *            +4·a(n‑7) −a(n‑8) for n≥8
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 20, u64 n <= 42, u128 n <= 87,
 *            big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_realloc.h"

static seq_t *cache = NULL;
static idx_t cache_size = 0;
static idx_t last_computed = 0;

static void ensure_cache_size(idx_t upto)
{
    if (upto + 1 > cache_size) {
        idx_t new_size = upto + 1;
        seq_t *tmp = seq_realloc(cache, cache_size, new_size);
        cache = tmp;
        cache_size = new_size;
    }
}

static void init_initial_terms(void)
{
    if (cache != NULL) return;                      /* already initialized */
    ensure_cache_size(7);                           /* indices 0..7 inclusive */
    cache[0] = (seq_t)1;
    cache[1] = (seq_t)6;
    cache[2] = (seq_t)17;
    cache[3] = (seq_t)58;
    cache[4] = (seq_t)188;
    cache[5] = (seq_t)570;
    cache[6] = (seq_t)1715;
    cache[7] = (seq_t)5074;
    last_computed = 7;
}

/* Compute a single term using the 8‑term recurrence.
   For 64‑bit seq_t, use 128‑bit intermediate to avoid overflow. */
static inline seq_t compute_term(idx_t i, seq_t *c)
{
#if defined(__SIZEOF_INT128__)
    /* If seq_t fits in 64 or 8 bytes, use unsigned __int128 for safety. */
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 pos = (unsigned __int128)c[i-1] * 4 +
                                (unsigned __int128)c[i-7] * 4;
        unsigned __int128 neg = (unsigned __int128)c[i-2] * 2 +
                                (unsigned __int128)c[i-4] * 11 +
                                (unsigned __int128)c[i-6] * 2 +
                                (unsigned __int128)c[i-8];
        return (seq_t)(pos - neg);
    }
#endif
    /* For arbitrary‑precision seq_t or if 128‑bit not available:
       perform all operations with seq_t itself. */
    seq_t pos = c[i-1];
    pos = pos * 4;
    seq_t tmp = c[i-7];
    tmp = tmp * 4;
    pos += tmp;

    seq_t neg = c[i-2];
    neg = neg * 2;
    tmp = c[i-4];
    tmp = tmp * 11;
    neg += tmp;
    tmp = c[i-6];
    tmp = tmp * 2;
    neg += tmp;
    neg += c[i-8];

    return pos - neg;
}

seq_t a(idx_t n)
{
    init_initial_terms();

    if (n <= 7)
        return cache[n];

    if (n > last_computed) {
        for (idx_t i = last_computed + 1; i <= n; ++i) {
            ensure_cache_size(i);          /* ensure capacity for this index */
            cache[i] = compute_term(i, cache);
        }
        last_computed = n;
    }
    return cache[n];
}
