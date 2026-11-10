/*
 * A277816  a(n) = the least k > n for which A264977(k) = A264977(n), or 0 if
 *          no such k exists.
 * https://oeis.org/A277816
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k > n with A264977(k) = A264977(n), else 0;
 *            computed by a search over a table of A264977 built with
 *            A(2m)=2A(m), A(2m+1)=A(m)^A(m+1), A(0)=0, A(1)=1
 * Range:     u8 n <= 23, u16 n <= 6143, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 98 terms through n = 9668 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint32_t *A277816_tab = 0;
static uint64_t A277816_len = 0;

static int A277816_grow(uint64_t newlen)
{
    uint32_t *p = (uint32_t *)realloc(A277816_tab, (size_t)newlen * sizeof(uint32_t));
    if (!p) return 0;
    A277816_tab = p;
    for (uint64_t i = A277816_len; i < newlen; i++) {
        if (i == 0) p[i] = 0;
        else if (i == 1) p[i] = 1;
        else if ((i & 1) == 0) p[i] = p[i >> 1] << 1;
        else p[i] = p[i >> 1] ^ p[(i >> 1) + 1];
    }
    A277816_len = newlen;
    return 1;
}

seq_t a(idx_t n)
{
    const uint64_t LIMIT = 1ULL << 26;
    if (n == 0) return 0;
    if ((uint64_t)n >= LIMIT) return 0;
    uint64_t need = 4096;
    while (need <= (uint64_t)n + 1) need <<= 1;
    if (A277816_len < need) {
        if (!A277816_grow(need)) return 0;
    }
    uint32_t target = A277816_tab[n];
    uint64_t k = (uint64_t)n + 1;
    for (;;) {
        for (; k < A277816_len; k++) {
            if (A277816_tab[k] == target) return (seq_t)k;
        }
        if (A277816_len >= LIMIT) return 0;
        if (!A277816_grow(A277816_len << 1)) return 0;
    }
}
