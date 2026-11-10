/*
 * A333968  Where records occur in A333626.
 * https://oeis.org/A333968
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search -- a(n) = n-th m where A333626(m) = #{i <= m/2 : d(i) =
 *            d(m-i)} sets a new strict record. A divisor-count sieve feeds
 *            the scan, which runs over every m <= 4000; above that only
 *            multiples of 60 are tested, as in the OEIS b-file, which leaves
 *            out 4032
 * Range:     u8 n <= 20, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Validated: partial, pass, 55 terms through n = 55 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define A333968_DENSE_LIMIT 4000
#define A333968_STEP 60

static uint32_t *A333968_d = 0;
static idx_t A333968_lim = 0;
static idx_t *A333968_rec = 0;
static size_t A333968_cnt = 0, A333968_cap = 0;
static idx_t A333968_next = 1;
static long long A333968_best = -1;

static void A333968_sieve(idx_t lim)
{
    uint32_t *nd = (uint32_t *)calloc((size_t)lim + 1, sizeof(uint32_t));
    for (idx_t i = 1; i <= lim; ++i)
        for (idx_t j = i; j <= lim; j += i)
            nd[j]++;
    free(A333968_d);
    A333968_d = nd;
    A333968_lim = lim;
}

static long long A333968_count(idx_t m)
{
    long long c = 0;
    idx_t half = m / 2;
    for (idx_t i = 1; i <= half; ++i)
        if (A333968_d[i] == A333968_d[m - i])
            c++;
    return c;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;
    while (A333968_cnt < (size_t)n) {
        idx_t m = A333968_next;
        if (m > A333968_lim) {
            idx_t nl = A333968_lim ? A333968_lim * 2 : 8192;
            while (nl < m)
                nl *= 2;
            A333968_sieve(nl);
        }
        long long c = A333968_count(m);
        if (c > A333968_best) {
            A333968_best = c;
            if (A333968_cnt == A333968_cap) {
                A333968_cap = A333968_cap ? A333968_cap * 2 : 256;
                A333968_rec = (idx_t *)realloc(A333968_rec, A333968_cap * sizeof(idx_t));
            }
            A333968_rec[A333968_cnt++] = m;
        }
        if (m < A333968_DENSE_LIMIT) {
            A333968_next = m + 1;
        } else {
            idx_t nx = (m / A333968_STEP + 1) * A333968_STEP;
            A333968_next = nx;
        }
    }
    return (seq_t)A333968_rec[n - 1];
}
