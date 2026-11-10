/*
 * A380391  Numbers k such that A343750(k) != k.
 * https://oeis.org/A380391
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th k >= 1 with A343750(k) != k; found by search, where
 *            A343750(k) is the digit permutation of k with the fewest
 *            divisors (ties: the smallest)
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Validated: partial, pass, 63 terms through n = 63 (2026-11-10)
 * Helpers:   num_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/num_divisors.h"

static void A380391_permute(uint8_t *d, int len, int pos, uint64_t *best, uint64_t *bestDiv)
{
    if (pos == len) {
        uint64_t val = 0;
        for (int i = 0; i < len; ++i) val = val * 10 + d[i];
        uint64_t c = num_divisors(val);
        if (c < *bestDiv || (c == *bestDiv && val < *best)) {
            *bestDiv = c;
            *best = val;
        }
        return;
    }
    for (int i = pos; i < len; ++i) {
        bool dup = false;
        for (int j = pos; j < i; ++j)
            if (d[j] == d[i]) { dup = true; break; }
        if (dup) continue;
        uint8_t t = d[pos]; d[pos] = d[i]; d[i] = t;
        A380391_permute(d, len, pos + 1, best, bestDiv);
        t = d[pos]; d[pos] = d[i]; d[i] = t;
    }
}

static uint64_t A380391_A343750(uint64_t k)
{
    uint8_t d[24];
    int len = 0;
    uint64_t t = k;
    while (t > 0) { d[len++] = (uint8_t)(t % 10); t /= 10; }
    for (int i = 0; i < len / 2; ++i) {
        uint8_t x = d[i]; d[i] = d[len - 1 - i]; d[len - 1 - i] = x;
    }
    uint64_t best = 0, bestDiv = UINT64_MAX;
    A380391_permute(d, len, 0, &best, &bestDiv);
    return best;
}

static uint64_t *A380391_terms = NULL;
static uint64_t A380391_count = 0, A380391_cap = 0, A380391_next = 1;

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    while (A380391_count < n) {
        uint64_t k = A380391_next++;
        if (A380391_A343750(k) != k) {
            if (A380391_count == A380391_cap) {
                uint64_t nc = A380391_cap ? A380391_cap * 2 : 256;
                A380391_terms = (uint64_t *)realloc(A380391_terms, nc * sizeof(uint64_t));
                A380391_cap = nc;
            }
            A380391_terms[A380391_count++] = k;
        }
    }
    return (seq_t)A380391_terms[n - 1];
}
