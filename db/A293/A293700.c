/*
 * A293700  First differences of A293698.
 * https://oeis.org/A293700
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A293698(n+1) - A293698(n), where A293698 lists the i >= 1
 *            with floor(tan(i)) = 1 (found by sequential search)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 93 terms through n = 9085 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdlib.h>
#include <stdint.h>

static uint64_t *A293700_cache = NULL;
static uint64_t A293700_len = 0;
static uint64_t A293700_cap = 0;
static uint64_t A293700_next = 1;

static void A293700_fill(uint64_t need)
{
    while (A293700_len < need) {
        double t = tan((double)A293700_next);
        if (t >= 1.0 && t < 2.0) {
            if (A293700_len == A293700_cap) {
                uint64_t nc = A293700_cap ? A293700_cap * 2 : 64;
                uint64_t *tmp = (uint64_t *)realloc(A293700_cache, nc * sizeof(uint64_t));
                if (!tmp) return;
                A293700_cache = tmp;
                A293700_cap = nc;
            }
            A293700_cache[A293700_len++] = A293700_next;
        }
        ++A293700_next;
    }
}

seq_t a(idx_t n)
{
    uint64_t k = (uint64_t)n;
    A293700_fill(k + 1);
    if (A293700_len < k + 1) return (seq_t)0;
    return (seq_t)(A293700_cache[k] - A293700_cache[k - 1]);
}
