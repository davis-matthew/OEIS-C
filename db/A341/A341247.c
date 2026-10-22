/*
 * A341247  Expansion of (-1 + Product_{k>=1} 1 / (1 + (-x)^k))^8.
 * https://oeis.org/A341247
 *
 * Offset:    8    (a(8) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in (-1 + \prod_{k>=1} (1 +
 *            x^{2*k-1}))^8, i.e. a(n) = coefficient of x^n in ( \sum_{m>=1}
 *            f(m) x^m )^8, where f(m) is the number of partitions of m into
 *            distinct odd parts
 * Range:     u8 n <= 14, u16 n <= 24, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   seq_alloc_new_array, seq_free
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation computes f(m) up to the requested n and then
   performs eight successive convolutions to obtain the desired
   coefficients.  All arrays are initialized to zero to avoid
   garbage from uninitialized memory. */

#include <stddef.h>
#include <stdint.h>
#include <string.h>              /* for memset */

#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free.h"

/* Cache for the computed values of a(n).  A_seq[0..cache_max] are valid. */
static seq_t *A_seq = NULL;
static idx_t cache_max = 0;

/* Allocate an array of seq_t initialized to zero. */
static seq_t *alloc_zeroed(idx_t sz)
{
    seq_t *p = seq_alloc_new_array(sz);
    memset(p, 0, sz * sizeof(seq_t));
    return p;
}

/* Recompute the sequence up to n (inclusive). */
static void recompute_cache(idx_t n)
{
    /* 1. partitions into distinct odd parts: f[0] = 1 (empty partition),
       but we will later drop f[0] because the GF is (P - 1). */
    seq_t *f = alloc_zeroed(n + 1);
    f[0] = 1;
    for (idx_t part = 1; part <= n; part += 2) {      /* odd part */
        for (idx_t sum = n; sum >= part; --sum) {
            f[sum] += f[sum - part];
        }
    }

    /* g holds f without the constant term; g[0] = 0. */
    seq_t *g = alloc_zeroed(n + 1);
    for (idx_t k = 1; k <= n; ++k) g[k] = f[k];

    /* 2. Convolution: w = g^8, computed iteratively. */
    seq_t *w = alloc_zeroed(n + 1);
    w[0] = 1;                       /* start with multiplicative identity */

    seq_t *tmp = alloc_zeroed(n + 1);

    for (int iter = 0; iter < 8; ++iter) {
        memset(tmp, 0, (n + 1) * sizeof(seq_t));
        for (idx_t i = 1; i <= n; ++i) {
            seq_t gi = g[i];
            if (gi == 0) continue;
            for (idx_t k = i; k <= n; ++k) {
                tmp[k] += gi * w[k - i];
            }
        }
        /* copy tmp back to w for the next iteration */
        memcpy(w, tmp, (n + 1) * sizeof(seq_t));
    }

    /* 3. Store the result into the new cache. */
    seq_t *new_cache = alloc_zeroed(n + 1);
    memcpy(new_cache, w, (n + 1) * sizeof(seq_t));

    /* Clean up temporaries. */
    seq_free(f);
    seq_free(g);
    seq_free(w);
    seq_free(tmp);

    /* Replace old cache. */
    seq_free(A_seq);
    A_seq = new_cache;
    cache_max = n;
}

/* Public entry point. */
seq_t a(idx_t n)
{
    if (n < 8) return 0;                /* sequence undefined below offset */

    if (n > cache_max) {
        recompute_cache(n);
    }
    return A_seq[n];
}
