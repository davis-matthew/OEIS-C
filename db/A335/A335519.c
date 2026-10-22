/*
 * A335519  Number of contiguous divisors of n.
 * https://oeis.org/A335519
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of distinct products of contiguous subsequences of
 *            the prime factor multiset of n (plus 1 for divisor 1)
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Compare two uint64_t values for qsort */
static int cmp_uint64(const void *a, const void *b)
{
    uint64_t aa = *(const uint64_t *)a;
    uint64_t bb = *(const uint64_t *)b;
    if (aa < bb) return -1;
    if (aa > bb) return 1;
    return 0;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* offset starts at 1; undefined otherwise */

    uint64_t nn = (uint64_t)n;
    uint64_t primes[64];
    size_t m = 0;

    /* factor out twos */
    while (nn % 2 == 0) {
        primes[m++] = 2;
        nn /= 2;
    }
    /* factor odd primes */
    for (uint64_t p = 3; p * p <= nn; p += 2) {
        while (nn % p == 0) {
            primes[m++] = p;
            nn /= p;
        }
    }
    if (nn > 1) primes[m++] = nn;

    /* n = 1 has only divisor 1 */
    if (m == 0) return (seq_t)1;

    /* total number of non‑empty contiguous subarrays */
    size_t total = m * (m + 1) / 2;
    uint64_t *arr = (uint64_t *)malloc(total * sizeof(uint64_t));
    if (!arr) return 0;   /* allocation failure – unlikely in tests */

    size_t cnt = 0;
    /* enumerate all contiguous subarrays and record their products */
    for (size_t i = 0; i < m; ++i) {
        uint64_t prod = 1;
        for (size_t j = i; j < m; ++j) {
            prod *= primes[j];
            arr[cnt++] = prod;
        }
    }

    /* sort and count distinct products */
    qsort(arr, cnt, sizeof(uint64_t), cmp_uint64);
    size_t uniq = 1;
    for (size_t i = 1; i < cnt; ++i) {
        if (arr[i] != arr[i - 1]) ++uniq;
    }
    free(arr);

    return (seq_t)(uniq + 1);  /* +1 for divisor 1 */
}
