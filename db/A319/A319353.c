/*
 * A319353  Filter sequence combining weights of partitions with Heinz-numbers
 *          equal to the proper divisors of n.
 * https://oeis.org/A319353
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = restricted growth sequence transform of A319352, where
 *            A319352(n) = Product_{d|n, d<n} prime(1+A056239(d)); computed by
 *            comparing the sorted multisets of weights A056239(d) of the
 *            proper divisors (equal multisets <=> equal products)
 * Range:     u8 n <= 371, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 115 terms through n = 9757 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

static int *cls_id = NULL;
static size_t cls_n = 0;

static void
a319353_build(size_t N)
{
    size_t m, d, k;
    int *spf = (int *)calloc(N + 1, sizeof(int));
    int *w = (int *)calloc(N + 1, sizeof(int));
    size_t *start = (size_t *)calloc(N + 2, sizeof(size_t));
    size_t *pos = (size_t *)calloc(N + 2, sizeof(size_t));
    int pidx = 0;
    for (m = 2; m <= N; m++) {
        if (spf[m] == 0) {
            pidx++;
            for (k = m; k <= N; k += m)
                if (spf[k] == 0) spf[k] = (int)m;
            w[m] = pidx;
        } else {
            w[m] = w[spf[m]] + w[m / spf[m]];
        }
    }
    w[1] = 0;
    for (d = 1; d <= N; d++)
        for (m = 2 * d; m <= N; m += d)
            start[m + 1]++;
    for (m = 1; m <= N; m++)
        start[m + 1] += start[m];
    /* start[m]..start[m+1] holds m's proper divisor weights */
    int *vals = (int *)malloc((start[N + 1] + 1) * sizeof(int));
    for (m = 0; m <= N + 1; m++) pos[m] = start[m];
    for (d = 1; d <= N; d++)
        for (m = 2 * d; m <= N; m += d)
            vals[pos[m]++] = w[d];
    for (m = 1; m <= N; m++) {
        size_t lo = start[m], hi = start[m + 1], i, j;
        for (i = lo + 1; i < hi; i++) {
            int v = vals[i];
            j = i;
            while (j > lo && vals[j - 1] > v) { vals[j] = vals[j - 1]; j--; }
            vals[j] = v;
        }
    }
    size_t H = 1;
    while (H < 2 * N + 2) H <<= 1;
    size_t *tab = (size_t *)calloc(H, sizeof(size_t));
    if (cls_id) free(cls_id);
    cls_id = (int *)calloc(N + 1, sizeof(int));
    int next = 0;
    for (m = 1; m <= N; m++) {
        uint64_t h = 1469598103934665603ULL;
        size_t lo = start[m], hi = start[m + 1], i;
        for (i = lo; i < hi; i++) {
            h ^= (uint64_t)(unsigned)vals[i] + 1;
            h *= 1099511628211ULL;
        }
        h ^= h >> 29;
        size_t slot = (size_t)(h & (H - 1));
        for (;;) {
            size_t r = tab[slot];
            if (r == 0) {
                tab[slot] = m;
                cls_id[m] = ++next;
                break;
            }
            if (start[r + 1] - start[r] == hi - lo) {
                bool eq = true;
                size_t rl = start[r];
                for (i = 0; i < hi - lo; i++)
                    if (vals[rl + i] != vals[lo + i]) { eq = false; break; }
                if (eq) { cls_id[m] = cls_id[r]; break; }
            }
            slot = (slot + 1) & (H - 1);
        }
    }
    cls_n = N;
    free(tab); free(vals); free(pos); free(start); free(w); free(spf);
}

seq_t
a(idx_t n)
{
    if ((size_t)n > cls_n) {
        size_t target = (size_t)n;
        if (target < 2 * cls_n) target = 2 * cls_n;
        if (target < 1024) target = 1024;
        a319353_build(target);
    }
    return (seq_t)cls_id[n];
}
