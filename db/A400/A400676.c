/*
 * A400676  First differences of A365874, divided by 4.
 * https://oeis.org/A400676
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini
 * Formula:   a(n) = (A365874(n+1) - A365874(n))/4, where A365874(1) = 1 and
 *            A365874(n) is the smallest integer > A365874(n-1) with
 *            A365874(n) - A365874(n-1) = 4*(10*(A365874(n-1) mod 10) +
 *            first_digit(A365874(n))) (iterative search)
 * Range:     u8 n <= 1, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Validated: partial, pass, 85 terms through n = 85 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"

static seq_t *A400676_cache = NULL;
static size_t A400676_cap = 0;
static size_t A400676_len = 0;
static bool A400676_ended = false;

static int A400676_first_digit(seq_t x)
{
    while (x >= 10) x /= 10;
    return (int)x;
}

static void A400676_ensure(size_t need)
{
    if (A400676_len >= need || A400676_ended) return;
    if (need > A400676_cap) {
        size_t nc = A400676_cap ? A400676_cap : 16;
        while (nc < need) nc <<= 1;
        seq_t *t = mem_resize(seq_t, A400676_cache, nc);
        if (!t) abort();
        A400676_cache = t;
        A400676_cap = nc;
    }
    if (A400676_len == 0) {
        A400676_cache[0] = 1;
        A400676_len = 1;
    }
    while (A400676_len < need) {
        seq_t prev = A400676_cache[A400676_len - 1];
        seq_t last = prev % 10;
        bool found = false;
        seq_t best = 0;
        for (int f = 1; f <= 9; ++f) {
            seq_t cand = prev + 4 * (10 * last + (seq_t)f);
            if (A400676_first_digit(cand) == f) {
                if (!found || cand < best) {
                    best = cand;
                    found = true;
                }
            }
        }
        if (!found) {
            A400676_ended = true;
            return;
        }
        A400676_cache[A400676_len++] = best;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    A400676_ensure((size_t)n + 1);
    if (A400676_len < (size_t)n + 1) return 0;
    return (A400676_cache[n] - A400676_cache[n - 1]) / 4;
}
