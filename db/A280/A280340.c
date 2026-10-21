/*
 * A280340  a(n) = a(n-1) + 10^n * a(n-2) with a(0) = 1 and a(1) = 1.
 * https://oeis.org/A280340
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 10^n * a(n-2), with a(0) = 1, a(1) = 1
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 11,
 *            big any n
 * Helpers:   grow_seq, mul_small, seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>                     /* size_t */
#include "../utils/grow_seq.h"           /* grow_seq */
#include "../utils/seq_mul.h"            /* seq_mul */
#include "../utils/mul_small.h"          /* mul_small */
#include "../utils/seq_pow.h"           /* seq_pow */

static seq_t *cache = NULL;   /* cache[0..max_cached-1] */
static size_t max_cached = 0; /* number of terms already computed */
static size_t cap = 0;        /* capacity of cache */

/* Return the n-th term of sequence A280340. */
seq_t a(idx_t n)
{
    /* Handle small indices directly. */
    if (n == 0) return 1;
    if (n == 1) return 1;

    /* Ensure space for up to n. */
    size_t need = (size_t)n + 1;
    if (!cache || need > cap)
        grow_seq(&cache, &cap, need);

    /* Initialise base terms if this is the first computation. */
    if (max_cached < 2) {
        cache[0] = 1;
        cache[1] = 1;
        max_cached = 2;
    }

    /* Compute missing terms up to n.  Use a running power of 10. */
    seq_t pow10 = seq_pow((seq_t)10, (idx_t)(max_cached - 1));

    for (idx_t i = (idx_t)max_cached; i <= n; ++i) {
        /* Update pow10 to 10^i. */
        pow10 = mul_small(pow10, 10u);               /* 10^{i-1} * 10 = 10^i */

        /* Compute a(i) = a(i-1) + 10^i * a(i-2). */
        seq_t prod = seq_mul(pow10, cache[i - 2]);   /* 10^i * a(i-2) */
        cache[i] = cache[i - 1] + prod;              /* a(i-1) + prod */

        ++max_cached;
    }

    return cache[n];
}
