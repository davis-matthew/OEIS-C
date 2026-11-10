/*
 * A245626  a(n)= 1 (respectively, a(n)= 3) if up to 2^n the number of
 *          A245622-terms is more (respectively, less) than the number of
 *          A245623-terms; or a(n)=0 if these numbers are equal.
 * https://oeis.org/A245626
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, more
 * Formula:   a(n) = 1 if pi_{1>3}(2^n) > pi_{3>1}(2^n), 3 if less, 0 if
 *            equal, where the counts are primes <= 2^n with more 1's than
 *            3's, or more 3's than 1's. Computed by a segmented sieve with a
 *            running difference recorded at each power of 2
 * Range:     u8 n <= 28, u16 n <= 28, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Validated: partial, pass, 29 terms through n = 28 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

static int A245626_res[70];
static int A245626_done = 1; /* results valid for indices 0..done */
static long long A245626_diff = 0;

static int A245626_cmp(uint64_t p)
{
    int c1 = 0, c3 = 0;
    while (p) {
        unsigned d = (unsigned)(p % 10U);
        if (d == 1) c1++;
        else if (d == 3) c3++;
        p /= 10U;
    }
    return c1 > c3 ? 1 : (c3 > c1 ? -1 : 0);
}

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)0;
    if (n > 62) n = 62;
    if ((int)n > A245626_done) {
        int target = (int)n;
        uint64_t blim = (uint64_t)1 << ((target + 1) / 2 + 1);
        unsigned char *bs = (unsigned char *)calloc((size_t)blim + 1, 1);
        uint32_t *bp = (uint32_t *)malloc(sizeof(uint32_t) * ((size_t)blim / 2 + 2));
        size_t bc = 0;
        uint64_t i, j;
        for (i = 3; i <= blim; i += 2) {
            if (!bs[i]) {
                bp[bc++] = (uint32_t)i;
                for (j = i * i; j <= blim; j += 2 * i) bs[j] = 1;
            }
        }
        free(bs);
        const size_t SEG = 1u << 19; /* odd numbers per segment */
        unsigned char *seg = (unsigned char *)malloc(SEG);
        int k;
        for (k = A245626_done + 1; k <= target; k++) {
            uint64_t s = ((uint64_t)1 << (k - 1)) + 1;
            uint64_t e = ((uint64_t)1 << k) - 1;
            if (k >= 2) {
                while (s <= e) {
                    uint64_t m = (e - s) / 2 + 1;
                    if (m > SEG) m = SEG;
                    uint64_t hi = s + 2 * (m - 1);
                    for (i = 0; i < m; i++) seg[i] = 0;
                    for (size_t q = 0; q < bc; q++) {
                        uint64_t p = bp[q];
                        if (p * p > hi) break;
                        uint64_t m0 = ((s + p - 1) / p) * p;
                        if (m0 < p * p) m0 = p * p;
                        if ((m0 & 1) == 0) m0 += p;
                        for (j = (m0 - s) / 2; j < m; j += p) seg[j] = 1;
                    }
                    for (i = 0; i < m; i++) {
                        if (!seg[i]) {
                            uint64_t v = s + 2 * i;
                            if (v >= 3) A245626_diff += A245626_cmp(v);
                        }
                    }
                    s = hi + 2;
                }
            }
            A245626_res[k] = A245626_diff > 0 ? 1 : (A245626_diff < 0 ? 3 : 0);
        }
        free(seg);
        free(bp);
        A245626_done = target;
    }
    return (seq_t)A245626_res[(int)n];
}
