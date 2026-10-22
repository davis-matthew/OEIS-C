/*
 * A326439  Number of maximal subsets of {1..n} such that no two elements have
 *          the same sorted prime signature.
 * https://oeis.org/A326439
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Π_{σ ∈ signatures(1..n)} count(σ)
 * Range:     u8 n <= 21, u16 n <= 53, u32 n <= 139, u64 n <= 478,
 *            u128 n <= 1000, big any n
 * Helpers:   build_spf, signature_key
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   where the signature of a number is the sorted multiset of its prime exponents   */

/* Include required utilities */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/build_spf.h"    /* provides build_spf */
#include "../utils/signature_key.h"

static int cmp_uint64(const void *a, const void *b)
{
    uint64_t aa = *(const uint64_t *)a;
    uint64_t bb = *(const uint64_t *)b;
    return (aa > bb) - (aa < bb);
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    /* Build smallest prime factor table up to n */
    idx_t *spf = NULL;
    build_spf(n, &spf);

    /* Allocate array for signatures */
    uint64_t *keys = (uint64_t *)malloc(sizeof(uint64_t) * (size_t)n);
    for (idx_t i = 1; i <= n; ++i) {
        keys[i - 1] = signature_key((uint64_t)i, spf);
    }

    /* Sort keys to count equal signatures */
    qsort(keys, (size_t)n, sizeof(uint64_t), cmp_uint64);

    seq_t prod = 1;
    size_t pos = 0;
    while (pos < (size_t)n) {
        uint64_t k = keys[pos];
        size_t j = pos + 1;
        while (j < (size_t)n && keys[j] == k) ++j;
        size_t cnt = j - pos;
        prod *= (seq_t)cnt;
        pos = j;
    }

    free(keys);
    free(spf);
    return prod;
}
