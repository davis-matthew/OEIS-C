/*
 * A256566  a(n) is the central term of the (2n-1)-st row of triangle A256440.
 * https://oeis.org/A256566
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = element at position n of row 2n-1 of A256440, i.e.
 *            integers 1..2n-1 sorted by sigma(k)/k (ties by k); computed by
 *            sieve + sort
 * Range:     u8 n <= 331, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 84 terms through n = 997 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static uint64_t *A256566_sig = NULL;

static int A256566_cmp(const void *pa, const void *pb)
{
    uint64_t a = *(const uint64_t *)pa;
    uint64_t b = *(const uint64_t *)pb;
    uint64_t l = A256566_sig[a] * b;
    uint64_t r = A256566_sig[b] * a;
    if (l < r) return -1;
    if (l > r) return 1;
    if (a < b) return -1;
    if (a > b) return 1;
    return 0;
}

seq_t a(idx_t n)
{
    uint64_t m = 2 * (uint64_t)n - 1;
    uint64_t *sig = (uint64_t *)malloc((size_t)(m + 1) * sizeof(uint64_t));
    uint64_t *nums = (uint64_t *)malloc((size_t)m * sizeof(uint64_t));
    if (!sig || !nums) {
        free(sig);
        free(nums);
        return 0;
    }
    for (uint64_t i = 0; i <= m; ++i) sig[i] = 0;
    for (uint64_t d = 1; d <= m; ++d)
        for (uint64_t j = d; j <= m; j += d)
            sig[j] += d;
    for (uint64_t i = 0; i < m; ++i) nums[i] = i + 1;
    A256566_sig = sig;
    qsort(nums, (size_t)m, sizeof(uint64_t), A256566_cmp);
    uint64_t res = nums[(size_t)(n - 1)];
    A256566_sig = NULL;
    free(sig);
    free(nums);
    return (seq_t)res;
}
