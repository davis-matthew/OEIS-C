/*
 * A361941  Inverse permutation to A361927.
 * https://oeis.org/A361941
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   A361927 is a square array A(i,j) read by antidiagonals (A(1,d),
 *            A(2,d-1), ..., A(d,1)). A(1,j) and A(i,1) are each the smallest
 *            number not yet used. Every other A(i,j) is the smallest unused
 *            multiple of lcm(A(i,1), A(1,j)). a(n) = the position of n in
 *            this reading of the array. The code builds th...
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Validated: partial, pass, 64 terms through n = 64 (2026-11-10)
 * Helpers:   gcd_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/gcd_u64.h"

static uint64_t *hk = NULL, *hv = NULL;
static size_t hcap = 0, hcnt = 0;

static size_t hslot(uint64_t key, size_t cap)
{
    return (size_t)((key * 0x9E3779B97F4A7C15ULL) >> 17) & (cap - 1);
}

static uint64_t hfind(uint64_t key)
{
    size_t i;
    if (hcap == 0) return 0;
    i = hslot(key, hcap);
    while (hk[i] != 0) {
        if (hk[i] == key) return hv[i];
        i = (i + 1) & (hcap - 1);
    }
    return 0;
}

static void hput_raw(uint64_t *K, uint64_t *V, size_t cap, uint64_t key, uint64_t val)
{
    size_t i = hslot(key, cap);
    while (K[i] != 0 && K[i] != key) i = (i + 1) & (cap - 1);
    K[i] = key;
    V[i] = val;
}

static void hinsert(uint64_t key, uint64_t val)
{
    if ((hcnt + 1) * 2 > hcap) {
        size_t ncap = hcap ? hcap * 2 : 1024, i;
        uint64_t *nk = (uint64_t *)calloc(ncap, sizeof(uint64_t));
        uint64_t *nv = (uint64_t *)calloc(ncap, sizeof(uint64_t));
        for (i = 0; i < hcap; i++)
            if (hk[i] != 0) hput_raw(nk, nv, ncap, hk[i], hv[i]);
        free(hk);
        free(hv);
        hk = nk;
        hv = nv;
        hcap = ncap;
    }
    hput_raw(hk, hv, hcap, key, val);
    hcnt++;
}

static uint64_t *row1 = NULL, *col1 = NULL;
static size_t rc_cap = 0;
static uint64_t D = 0, Kpos = 0, low = 1;

static void next_diag(void)
{
    uint64_t d = ++D, i;
    if (d + 1 >= rc_cap) {
        size_t ncap = rc_cap ? rc_cap * 2 : 256;
        while (ncap <= d + 1) ncap *= 2;
        row1 = (uint64_t *)realloc(row1, ncap * sizeof(uint64_t));
        col1 = (uint64_t *)realloc(col1, ncap * sizeof(uint64_t));
        rc_cap = ncap;
    }
    for (i = 1; i <= d; i++) {
        uint64_t j = d + 1 - i, v;
        if (i == 1 || j == 1) {
            while (hfind(low)) low++;
            v = low;
        } else {
            uint64_t x = col1[i], y = row1[j];
            uint64_t L = x / gcd_u64(x, y) * y;
            v = L;
            while (hfind(v)) v += L;
        }
        if (i == 1) row1[j] = v;
        if (j == 1) col1[i] = v;
        hinsert(v, ++Kpos);
    }
}

seq_t a(idx_t n)
{
    uint64_t p;
    while ((p = hfind((uint64_t)n)) == 0) next_diag();
    return (seq_t)p;
}
