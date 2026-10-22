/*
 * A362895  a(n) is the length of the smallest orbit of the n-th natural
 *          downset.
 * https://oeis.org/A362895
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(j)*binomial(k(n),k(j)) where k(n) = ceil(log₂(n)) for
 *            n>0, j = 2^k(n) – n, k(j) = ceil(log₂(j)) if j>0 else 0, with
 *            a(0) = 1
 * Range:     u8 n <= 41, u16 n <= 297, u32 n <= 8873, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   binomial, ceil_log2_nonzero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/ceil_log2_nonzero.h"

/* Memoisation cache: cache[i]==0 means not yet computed (values are all >0).  */
static seq_t *cache = NULL;
static idx_t cache_size = 0;

static void ensure_capacity(idx_t n)
{
    if (n < cache_size) return;
    idx_t new_size = cache_size ? cache_size * 2 : 8;
    while (new_size <= n) new_size *= 2;
    seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) abort();
    for (idx_t i = cache_size; i < new_size; ++i) tmp[i] = 0;
    cache = tmp;
    cache_size = new_size;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;          /* a(0) = 1 */
    ensure_capacity(n);
    if (cache[n] != 0) return cache[n];   /* already computed */

    idx_t k  = ceil_log2_nonzero(n);
    idx_t powk = 1ULL << k;
    idx_t j  = powk - n;

    seq_t a_j = (j == 0) ? (seq_t)1 : a(j);

    idx_t k_j = (j == 0) ? 0 : ceil_log2_nonzero(j);
    seq_t bin = binomial(k, k_j);

    seq_t res = a_j * bin;
    cache[n] = res;
    return res;
}
