/*
 * A357809  Locations of successive records in A357062.
 * https://oeis.org/A357809
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, hard
 * Formula:   a(n) = location (index m) of the n-th strict record in A357062
 *            (number of ordered solutions to m = x*y*z+x+y+z), counting m=0
 *            as the first record; computed by sieving solution counts over
 *            x<=y<=z with a doubling limit
 * Range:     u8 n <= 15, u16 n <= 45, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Validated: partial, pass, 47 terms through n = 47 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static uint64_t *A357809_rec = 0;
static size_t A357809_nrec = 0;
static uint64_t A357809_limit = 0;

static void A357809_build(uint64_t L)
{
    uint32_t *cnt = (uint32_t *)calloc((size_t)L + 1, sizeof(uint32_t));
    if (!cnt) return;
    for (uint64_t x = 1; x * x * x + 3 * x <= L; ++x) {
        for (uint64_t y = x; ; ++y) {
            uint64_t step = x * y + 1;
            uint64_t m = y * step + x + y;
            if (m > L) break;
            for (uint64_t z = y; m <= L; ++z, m += step) {
                uint32_t mult;
                if (x == y && y == z) mult = 1;
                else if (x == y || y == z) mult = 3;
                else mult = 6;
                cnt[m] += mult;
            }
        }
    }
    free(A357809_rec);
    A357809_rec = (uint64_t *)malloc(sizeof(uint64_t) * 4096);
    size_t cap = 4096;
    A357809_nrec = 0;
    int64_t best = -1;
    for (uint64_t m = 0; m <= L; ++m) {
        if ((int64_t)cnt[m] > best) {
            best = (int64_t)cnt[m];
            if (A357809_nrec == cap) {
                cap *= 2;
                A357809_rec = (uint64_t *)realloc(A357809_rec, sizeof(uint64_t) * cap);
            }
            A357809_rec[A357809_nrec++] = m;
        }
    }
    free(cnt);
    A357809_limit = L;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    if (A357809_limit == 0) A357809_build(1u << 14);
    while (A357809_nrec < (size_t)n) {
        uint64_t old = A357809_limit;
        A357809_build(old * 2);
        if (A357809_limit == old) break;
    }
    if (A357809_nrec < (size_t)n) return 0;
    return (seq_t)A357809_rec[n - 1];
}
