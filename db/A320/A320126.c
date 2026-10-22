/*
 * A320126  Number of integer solutions to a^2 + b^2 + 2*c^2 + 5*d^2 = n.
 * https://oeis.org/A320126
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integer solutions to a^2 + b^2 + 2*c^2 + 5*d^2
 *            = n
 * Range:     u8 n <= 56, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Integer floor square root for idx_t (unsigned) */
static idx_t int_sqrt(idx_t n)
{
    idx_t lo = 0, hi = n;
    while (lo < hi) {
        idx_t mid = lo + ((hi - lo + 1) >> 1);
        if (mid <= n / mid)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* Allocate a zero‑initialized array of seq_t */
static seq_t *alloc_seq_array(size_t sz)
{
    return (seq_t *)calloc(sz, sizeof(seq_t));
}

/* Cached tables of representations of a^2 + b^2 and 2*c^2 + 5*d^2 */
static seq_t *fAB = NULL;
static seq_t *fCD = NULL;
static idx_t cachedN = 0;

/* Rebuild the caches up to (and including) n */
static void rebuild_cache(idx_t n)
{
    size_t sz = (size_t)(n + 1);
    seq_t *newAB = alloc_seq_array(sz);
    seq_t *newCD = alloc_seq_array(sz);

    /* Build fAB: a^2 + b^2 */
    idx_t aMax = int_sqrt(n);
    for (idx_t a = 0; a <= aMax; ++a) {
        idx_t a2 = a * a;
        idx_t remMax = n - a2;
        idx_t bMax = int_sqrt(remMax);
        for (idx_t b = 0; b <= bMax; ++b) {
            idx_t b2 = b * b;
            idx_t sum = a2 + b2;
            seq_t mult = (a == 0 && b == 0) ? 1
                        : ((a == 0) || (b == 0)) ? 2
                        : 4;
            newAB[sum] += mult;
        }
    }

    /* Build fCD: 2*c^2 + 5*d^2 */
    idx_t cMax = int_sqrt(n / 2);
    for (idx_t c = 0; c <= cMax; ++c) {
        idx_t c2 = c * c;
        idx_t valC = 2 * c2;
        if (valC > n) break;
        idx_t remMaxD = n - valC;
        idx_t dMax = int_sqrt(remMaxD / 5);
        for (idx_t d = 0; d <= dMax; ++d) {
            idx_t d2 = d * d;
            idx_t valD = 5 * d2;
            idx_t sum = valC + valD;
            seq_t mult = (c == 0 && d == 0) ? 1
                        : ((c == 0) || (d == 0)) ? 2
                        : 4;
            newCD[sum] += mult;
        }
    }

    free(fAB);
    free(fCD);
    fAB = newAB;
    fCD = newCD;
    cachedN = n;
}

seq_t a(idx_t n)
{
    /* Ensure caches exist and are up to date for this n */
    if (fAB == NULL) {
        rebuild_cache(n);
    } else if (n > cachedN) {
        rebuild_cache(n);
    }

    seq_t result = 0;
    for (idx_t s = 0; s <= n; ++s) {
        result += fAB[n - s] * fCD[s];
    }
    return result;
}
