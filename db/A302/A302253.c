/*
 * A302253  Positions of 3 in A190436.
 * https://oeis.org/A302253
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th positive k with A190436(k) = 3, where A190436(k) =
 *            floor((3k+2)*phi) - 3*floor(k*phi) - 3, phi = (1+sqrt(5))/2;
 *            computed by a search
 * Range:     u8 n <= 20, u16 n <= 5158, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 74 terms through n = 9679 (2026-11-08)
 * Helpers:   isqrt_u128_to_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/isqrt_u128_to_u64.h"

static uint64_t A302253_floor_phi(uint64_t k)
{
    unsigned __int128 v = (unsigned __int128)k * k * 5;
    uint64_t s = isqrt_u128_to_u64(v);
    return (s + k) / 2;
}

static int A302253_val(uint64_t k)
{
    uint64_t t1 = A302253_floor_phi(3 * k + 2);
    uint64_t t2 = A302253_floor_phi(k);
    return (int)(t1 - 3 * t2 - 3);
}

static uint64_t *A302253_cache = NULL;
static uint64_t A302253_len = 0;
static uint64_t A302253_cap = 0;
static uint64_t A302253_last = 0;

seq_t a(idx_t n)
{
    uint64_t need = (uint64_t)n;
    if (need > A302253_len) {
        if (need > A302253_cap) {
            uint64_t nc = A302253_cap ? A302253_cap : 64;
            while (nc < need) nc *= 2;
            A302253_cache = (uint64_t *)realloc(A302253_cache, sizeof(uint64_t) * nc);
            A302253_cap = nc;
        }
        uint64_t k = A302253_last;
        while (A302253_len < need) {
            k++;
            if (A302253_val(k) == 3) {
                A302253_cache[A302253_len++] = k;
            }
        }
        A302253_last = k;
    }
    return (seq_t)A302253_cache[need - 1];
}
