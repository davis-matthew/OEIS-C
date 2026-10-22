/*
 * A338623  a(n) is the length of the longest block of consecutive terms
 *          appearing twice (possibly with overlap) among the first n terms of
 *          the Thue-Morse sequence (A010060).
 * https://oeis.org/A338623
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 639, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Calculates a(n) for OEIS A338623:
   the length of the longest block of consecutive terms that appears
   at least twice (possibly overlapping) in the first n terms of the
   Thue–Morse sequence.  This uses a suffix‑array + LCP array
   algorithm in O(n log n). */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* static data for qsort comparator */
static const int *cmp_rank;
static int cmp_k;
static int cmp_len;

static int cmp_sa(const void *p, const void *q)
{
    int i = *(const int *)p;
    int j = *(const int *)q;
    int ri = cmp_rank[i];
    int rj = cmp_rank[j];
    if (ri != rj) return ri - rj;
    int ri2 = (i + cmp_k < cmp_len ? cmp_rank[i + cmp_k] : -1);
    int rj2 = (j + cmp_k < cmp_len ? cmp_rank[j + cmp_k] : -1);
    return ri2 - rj2;
}

/* parity of bit count of x */
static inline int bit_parity(uint64_t x)
{
    x ^= x >> 32;
    x ^= x >> 16;
    x ^= x >> 8;
    x ^= x >> 4;
    x ^= x >> 2;
    x ^= x >> 1;
    return x & 1U;
}

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)0;

    size_t len = (size_t)n;

    /* Thue–Morse sequence array */
    char *t = (char *)malloc(len * sizeof(char));
    for (size_t i = 0; i < len; ++i)
        t[i] = (char)bit_parity((uint64_t)i);

    /* suffix array construction */
    int *sa  = (int *)malloc(len * sizeof(int));
    int *rank  = (int *)malloc(len * sizeof(int));
    int *tmp  = (int *)malloc(len * sizeof(int));

    for (size_t i = 0; i < len; ++i) {
        sa[i] = (int)i;
        rank[i] = t[i] & 1;   /* 0 or 1 */
    }

    for (int k = 1; k < (int)len; k <<= 1) {
        cmp_rank = rank;
        cmp_k = k;
        cmp_len = (int)len;
        qsort(sa, len, sizeof(int), cmp_sa);

        tmp[sa[0]] = 0;
        for (size_t i = 1; i < len; ++i) {
            int prev = sa[i - 1], cur = sa[i];
            int diff = (rank[prev] != rank[cur]) ||
                       ((prev + k < len ? rank[prev + k] : -1) !=
                        (cur + k < len ? rank[cur + k] : -1));
            tmp[cur] = tmp[prev] + (diff ? 1 : 0);
        }
        for (size_t i = 0; i < len; ++i)
            rank[i] = tmp[i];
        if (rank[sa[len - 1]] == (int)len - 1) break;
    }

    /* rank array for Kasai */
    int *rpos = (int *)malloc(len * sizeof(int));
    for (size_t i = 0; i < len; ++i)
        rpos[sa[i]] = (int)i;

    /* compute LCP and find maximum */
    int h = 0;
    int maxlcp = 0;
    for (size_t i = 0; i < len; ++i) {
        int r = rpos[i];
        if (r > 0) {
            int j = sa[r - 1];
            while (i + h < len && j + h < len && t[i + h] == t[j + h]) h++;
            if (h > maxlcp) maxlcp = h;
            if (h > 0) h--;
        }
    }

    free(t);
    free(sa);
    free(rank);
    free(tmp);
    free(rpos);

    return (seq_t)maxlcp;
}
