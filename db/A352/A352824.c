/*
 * A352824  Number of fixed points y(i) = i, where y is the integer partition
 *          with Heinz number n.
 * https://oeis.org/A352824
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of positions i in the partition (with Heinz number
 *            n) such that the i‑th part equals i (parts sorted in
 *            non‑increasing order)
 * Range:     u8 n <= 108, u16 n <= 108, u32 n <= 108, u64 n <= 108,
 *            u128 n <= 108, big any n
 * Helpers:   build_primes_upto, insertion_sort_desc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/build_primes_upto.h"
#include "../utils/insertion_sort_desc.h"

/* static storage for primes up to current limit */
static uint64_t *primes = NULL;
static size_t    prime_cnt = 0;
static idx_t    prime_limit = 0;

/* ensures that primes array contains all primes ≤ limit */
static void primes_upto(idx_t limit)
{
    if (limit <= prime_limit) return;     /* already sufficient */

    uint64_t *tmp = NULL;
    size_t    cnt  = 0;
    build_primes_upto(limit, &tmp, &cnt);   /* tmp points to new array */

    /* replace old array */
    free(primes);
    primes = tmp;
    prime_cnt = cnt;
    prime_limit = limit;
}

/* binary search for prime index (1‑based) */
static idx_t prime_index(uint64_t p)
{
    size_t lo = 0, hi = prime_cnt;
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        uint64_t pm = primes[mid];
        if (pm == p) return (idx_t)(mid + 1);   /* indices are 1‑based */
        if (pm < p)
            lo = mid + 1;
        else
            hi = mid;
    }
    return 0;   /* should never happen for prime p */
}

/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;          /* partition of 1 is empty */

    /* make sure we have all primes up to n */
    primes_upto(n);

    uint64_t nn = (uint64_t)n;
    /* estimate capacity: number of prime factors <= log2(n)+1 */
    size_t cap = 16;
    size_t len = 0;
    uint64_t *idxs = (uint64_t *)malloc(cap * sizeof(uint64_t));
    if (!idxs) return (seq_t)0; /* allocation failed, return 0 */

    /* factor nn using trial division over precomputed primes */
    for (size_t pi = 0; pi < prime_cnt && (uint64_t)primes[pi] * (uint64_t)primes[pi] <= nn; ++pi) {
        uint64_t p = primes[pi];
        while (nn % p == 0) {
            if (len == cap) {
                cap <<= 1;
                uint64_t *tmp = (uint64_t *)realloc(idxs, cap * sizeof(uint64_t));
                if (!tmp) { free(idxs); return (seq_t)0; }
                idxs = tmp;
            }
            idxs[len++] = prime_index(p);
            nn /= p;
        }
    }
    if (nn > 1) {   /* remaining prime factor > sqrt(original n) */
        if (len == cap) {
            cap <<= 1;
            uint64_t *tmp = (uint64_t *)realloc(idxs, cap * sizeof(uint64_t));
            if (!tmp) { free(idxs); return (seq_t)0; }
            idxs = tmp;
        }
        idxs[len++] = prime_index(nn);
    }

    if (len == 0) { free(idxs); return (seq_t)0; }

    /* sort indices descending to form canonical partition */
    insertion_sort_desc(idxs, len);

    /* count fixed points */
    size_t count = 0;
    for (size_t i = 0; i < len; ++i) {
        if (idxs[i] == (uint64_t)(i + 1))
            ++count;
    }

    free(idxs);
    return (seq_t)count;
}
