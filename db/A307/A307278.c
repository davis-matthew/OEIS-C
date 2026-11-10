/*
 * A307278  Record values in A050782.
 * https://oeis.org/A307278
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   search -- a(n) is the n-th record value of A050782(k), k =
 *            0,1,2,... (a(1) = A050782(0) = 0). A050782(k) is the least
 *            palindrome divisible by k, divided by k. For each length L, a
 *            residue DP mod k finds the smallest palindrome of that length,
 *            digit by digit from the most significant end
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 10, u64 n <= 11, u128 n <= 12,
 *            big any n
 * Validated: partial, pass, 12 terms through n = 12 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* Smallest m >= 1 with m*k a decimal palindrome, or 0 if k % 10 == 0 */
static seq_t A050782_fast(uint64_t k)
{
    if (k % 10u == 0u)
        return (seq_t)0;
    for (int L = 1; ; ++L) {
        int h = (L + 1) / 2;
        uint64_t *p = (uint64_t *)malloc(sizeof(uint64_t) * (size_t)L);
        uint64_t *w = (uint64_t *)malloc(sizeof(uint64_t) * (size_t)h);
        unsigned char *reach =
            (unsigned char *)calloc((size_t)(h + 1) * (size_t)k, 1);
        unsigned char *dg = (unsigned char *)malloc((size_t)L);
        p[0] = 1u % k;
        for (int i = 1; i < L; ++i)
            p[i] = (p[i - 1] * 10u) % k;
        for (int i = 0; i < h; ++i) {
            int j = L - 1 - i;              /* i counts from the most significant end */
            if (i == j)
                w[i] = p[i];
            else
                w[i] = (p[i] + p[j]) % k;
        }
        reach[(size_t)h * k + 0] = 1;
        for (int i = h - 1; i >= 0; --i) {
            int lo = (i == 0) ? 1 : 0;
            unsigned char *nx = reach + (size_t)(i + 1) * k;
            unsigned char *cu = reach + (size_t)i * k;
            for (uint64_t r = 0; r < k; ++r) {
                if (!nx[r]) continue;
                for (int d = lo; d <= 9; ++d)
                    cu[(r + (uint64_t)d * w[i]) % k] = 1;
            }
        }
        if (reach[0]) {
            uint64_t need = 0;
            for (int i = 0; i < h; ++i) {
                int lo = (i == 0) ? 1 : 0;
                for (int d = lo; d <= 9; ++d) {
                    uint64_t rest = (need + k - ((uint64_t)d * w[i]) % k) % k;
                    if (reach[(size_t)(i + 1) * k + rest]) {
                        dg[i] = (unsigned char)d;
                        need = rest;
                        break;
                    }
                }
            }
            for (int j = h; j < L; ++j)
                dg[j] = dg[L - 1 - j];
            /* long division of the palindrome digits by k */
            uint64_t r = 0;
            seq_t q = (seq_t)0;
            for (int j = 0; j < L; ++j) {
                uint64_t cur = r * 10u + dg[j];
                q = q * (seq_t)10 + (seq_t)(cur / k);
                r = cur % k;
            }
            free(p); free(w); free(reach); free(dg);
            return q;
        }
        free(p); free(w); free(reach); free(dg);
    }
}

static seq_t *A307278_recs = NULL;
static idx_t A307278_nrec = 0;
static idx_t A307278_cap = 0;
static idx_t A307278_nextk = 1;

seq_t a(idx_t n)
{
    if (n < (idx_t)1)
        return (seq_t)0;
    if (A307278_nrec == 0) {
        A307278_cap = 16;
        A307278_recs = mem_new(seq_t, A307278_cap);
        A307278_recs[0] = (seq_t)0;
        A307278_nrec = 1;
    }
    while (A307278_nrec < n) {
        seq_t m = A050782_fast((uint64_t)A307278_nextk);
        ++A307278_nextk;
        if (m > A307278_recs[A307278_nrec - 1]) {
            if (A307278_nrec == A307278_cap) {
                A307278_cap *= 2;
                A307278_recs = mem_resize(seq_t, A307278_recs, A307278_cap);
            }
            A307278_recs[A307278_nrec++] = m;
        }
    }
    return A307278_recs[n - 1];
}
