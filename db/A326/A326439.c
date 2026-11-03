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
 * Validated: standard, pass, 67 terms through n = 991 (2026-11-01)
 * Helpers:   build_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   where the signature of a number is the sorted multiset of its prime exponents   */

/* Include required utilities */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/build_spf.h"

/* Compute the encoded key of the prime signatures of val using the SPF array */
static uint64_t signature_key(uint64_t val, idx_t *spf)
{
    uint8_t exps[64];
    size_t len = 0;
    while (val > 1) {
        idx_t p = spf[(idx_t)val];
        uint8_t cnt = 0;
        while (val % p == 0) {
            val /= p;
            cnt++;
        }
        exps[len++] = cnt;
    }
    /* sort the exponents ascending – tiny array – insertion sort */
    for (size_t i = 1; i < len; ++i) {
        uint8_t key = exps[i];
        size_t j = i;
        while (j > 0 && exps[j - 1] > key) {
            exps[j] = exps[j - 1];
            --j;
        }
        exps[j] = key;
    }
    /* encode key: len in lowest 4 bits, each exponent in 4 bits */
    uint64_t k = (uint64_t)len & 0xFULL;
    for (size_t i = 0; i < len; ++i) {
        k |= ((uint64_t)(exps[i] & 0xF) << (4 * (i + 1)));
    }
    return k;
}

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
