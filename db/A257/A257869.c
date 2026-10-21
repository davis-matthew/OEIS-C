/*
 * A257869  Nonnegative integers with an equal number of occurrences of all
 *          trits in balanced ternary representation.
 * https://oeis.org/A257869
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 592, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   next_comb
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A257869: numbers with an equal number of occurrences of the trits
    -1, 0 and +1 in their balanced ternary representation.
    The function enumerates all such numbers in increasing order and
    caches the values for repeated calls.  */

/* Standard headers and integral types provided by the build. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/next_comb.h"

/* ---------------------------------------------------------------------- */
/* Helper: compare two seq_t values for qsort. */
static int cmp_seq_t(const void *a, const void *b)
{
    const seq_t *pa = (const seq_t *)a;
    const seq_t *pb = (const seq_t *)b;
    if (*pa < *pb) return -1;
    if (*pa > *pb) return 1;
    return 0;
}

/* ---------------------------------------------------------------------- */

/* ---------------------------------------------------------------------- */
/* Compute binomial coefficient C(n,k) using only integer arithmetic. */
static seq_t binomial(seq_t n, seq_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;                /* use symmetry */
    seq_t res = 1;
    for (seq_t i = 1; i <= k; ++i)
        res = res * (n - i + 1) / i;
    return res;
}

/* ---------------------------------------------------------------------- */
/* Generate all numbers of length 3k with equal numbers of -1,0,+1,
   with the most significant trit fixed to +1.
   The generated values are stored in dest[0..count_k-1] and then sorted. */
static void generate_k(idx_t k, seq_t *dest)
{
    size_t L = 3 * k;                /* total digits */
    size_t positions = L - 1;        /* exclude leading digit */

    /* powers of 3 (weights) */
    seq_t pow3[64];
    pow3[0] = 1;
    for (size_t i = 1; i < L; ++i)
        pow3[i] = pow3[i - 1] * (seq_t)3;

    /* value if all digits were +1 */
    seq_t baseAllOnes = 0;
    for (size_t i = 0; i < L; ++i)
        baseAllOnes += pow3[i];

    /* combination arrays */
    size_t minusComb[64];
    size_t zeroComb[64];
    size_t remainIdx[64];

    /* initial minusComb: first k positions 0..k-1 */
    for (size_t j = 0; j < k; ++j)
        minusComb[j] = j;

    size_t offset = 0;
    do {
        /* positions not used by -1 trits */
        size_t rcount = 0;
        for (size_t pos = 0; pos < positions; ++pos) {
            bool inMinus = false;
            for (size_t j = 0; j < k; ++j) {
                if (minusComb[j] == pos) { inMinus = true; break; }
            }
            if (!inMinus)
                remainIdx[rcount++] = pos;
        }

        /* initial zeroComb: first k positions of remainIdx */
        for (size_t j = 0; j < k; ++j)
            zeroComb[j] = j;

        do {
            seq_t val = baseAllOnes;
            /* subtract contribution of -1 trits (difference -2*3^w) */
            for (size_t j = 0; j < k; ++j) {
                size_t pos = minusComb[j] + 1;          /* real digit position */
                val -= (seq_t)2 * pow3[L - 1 - pos];
            }
            /* subtract contribution of 0 trits (difference -1*3^w) */
            for (size_t j = 0; j < k; ++j) {
                size_t pos = remainIdx[zeroComb[j]] + 1;
                val -= pow3[L - 1 - pos];
            }
            dest[offset++] = val;
        } while (next_comb(zeroComb, (size_t)k, rcount));
    } while (next_comb(minusComb, (size_t)k, positions));

    /* sort the generated values for this k */
    qsort(dest, offset, sizeof(seq_t), cmp_seq_t);
}

/* ---------------------------------------------------------------------- */
/* Cache for previously computed sequence values. */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static idx_t last_k = 0;      /* highest k that has been generated */

/* ---------------------------------------------------------------------- */
/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;            /* undefined for n==0 – avoid crash */

/* Return cached value if already computed. */
    if (n <= cache_len)
        return cache[n - 1];

/* Generate additional terms until we have at least n values. */
    for (idx_t k = last_k + 1; ; ++k) {
        /* count of terms for this k: C(3k-1,k) * C(2k-1,k) */
        seq_t c1 = binomial((seq_t)(3 * k - 1), (seq_t)k);
        seq_t c2 = binomial((seq_t)(2 * k - 1), (seq_t)k);
        seq_t c  = c1 * c2;
        size_t count_k = (size_t)c;

        size_t newSize = cache_len + count_k;
        seq_t *newCache = (seq_t *)realloc(cache, newSize * sizeof(seq_t));
        if (!newCache) {
            /* allocation failure – abort */
            exit(1);
        }
        cache = newCache;
        generate_k(k, cache + cache_len);
        cache_len = newSize;
        last_k = k;

        if (cache_len >= n)
            break;
    }
    return cache[n - 1];
}
