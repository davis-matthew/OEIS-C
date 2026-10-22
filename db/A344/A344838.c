/*
 * A344838  Square array T(n, k), n, k >= 0, read by antidiagonals; T(n, k) =
 *          max(n * 2^max(0, w(k)-w(n)), k * 2^max(0, w(n)-w(k))) (where w =
 *          A070939).
 * https://oeis.org/A344838
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, tabl
 * Formula:   a(n) is obtained by reading the square array T(n,k) along *
 *            antidiagonals. T(n,k) is defined by * * T(n,k) = max( n *
 *            2^max(0,w(k)-w(n)), k * 2^max(0,w(n)-w(k)) ) * * where w(x) is
 *            the bit‑length of x (w(0)=0). This value is the * n‑th term a(n)
 *            with a(0)=0
 * Range:     u8 n <= 9053, u16 n <= 9053, u32 n <= 9053, u64 n <= 9053,
 *            u128 n <= 9053, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* bit length of an idx_t (0 -> 0, otherwise floor(log2(v))+1) */
static inline unsigned bitlen_idx(idx_t v)
{
    return v ? 64 - __builtin_clzll(v) : 0;
}

/* compute T(n,k) as described above */
static inline seq_t T(idx_t n, idx_t k)
{
    /* Special cases handled by the general formulation */
    seq_t nn = (seq_t)n;
    seq_t kk = (seq_t)k;

    unsigned bn = bitlen_idx(n);
    unsigned bk = bitlen_idx(k);
    int diff = (int)bk - (int)bn;

    if (diff > 0) {                /* shift n */
        seq_t a = nn << diff;
        seq_t b = kk;
        return a > b ? a : b;
    } else if (diff < 0) {         /* shift k */
        seq_t a = nn;
        seq_t b = kk << (-diff);
        return a > b ? a : b;
    } else {                       /* equal bit length */
        return nn > kk ? nn : kk;
    }
}

/* cached sequence */
static seq_t *cache = NULL;           /* array of computed terms      */
static idx_t cache_cap = 0;           /* capacity of the cache       */
static idx_t next_idx = 0;            /* next index to compute        */
static idx_t diag = 0;                /* current antidiagonal        */
static idx_t i = 0;                  /* n coordinate within diag    */

/* a(n) – sequence A344838 */
seq_t a(idx_t n)
{
    /* enlarge cache if necessary */
    if (n >= cache_cap) {
        idx_t need = (n + 1) * 2;          /* double for growth factor */
        if (need < n + 1)
            need = n + 1;                  /* overflow guard */
        seq_t *tmp = (seq_t *)realloc(cache, need * sizeof(seq_t));
        if (!tmp)
            abort();                       /* out of memory */
        cache = tmp;
        cache_cap = need;
    }

    /* compute missing terms up to index n */
    while (next_idx <= n) {
        seq_t val = T(i, diag - i);
        cache[next_idx++] = val;

        if (i < diag)
            ++i;
        else {
            ++diag;
            i = 0;
        }
    }

    return cache[n];
}
