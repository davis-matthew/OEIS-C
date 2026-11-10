/*
 * A370794  Inverse permutation to A370793.
 * https://oeis.org/A370794
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look, base
 * Formula:   a(n) = m such that A370793(m) = n (inverse permutation); A370793
 *            generated greedily: A370793(0)=0, then smallest unused k with
 *            (prev AND k) in {0, prev}
 * Range:     u8 n <= 177, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 84 terms through n = 9773 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

static uint64_t *inv_pos = NULL;   /* inv_pos[v] = index+1 where value v occurs, 0 if not yet */
static uint64_t inv_cap = 0;
static uint64_t inv_len = 0;       /* number of generated terms */
static uint64_t inv_cur = 0;       /* last generated term */
static uint64_t inv_low = 0;       /* smallest unused value */

static void inv_ensure(uint64_t v)
{
    if (v < inv_cap) return;
    uint64_t nc = inv_cap ? inv_cap : 1024;
    while (nc <= v) nc <<= 1;
    uint64_t *p = (uint64_t *)calloc((size_t)nc, sizeof(uint64_t));
    if (!p) abort();
    if (inv_pos) {
        memcpy(p, inv_pos, (size_t)inv_cap * sizeof(uint64_t));
        free(inv_pos);
    }
    inv_pos = p;
    inv_cap = nc;
}

static void inv_step(void)
{
    if (inv_len == 0) {
        inv_ensure(0);
        inv_pos[0] = 1;
        inv_len = 1;
        inv_cur = 0;
        inv_low = 1;
        return;
    }
    uint64_t c = inv_low;
    for (;; ++c) {
        if (c < inv_cap && inv_pos[c]) continue;
        uint64_t r = inv_cur & c;
        if (r == 0 || r == inv_cur) break;
    }
    inv_ensure(c);
    inv_pos[c] = inv_len + 1;
    inv_len++;
    inv_cur = c;
    while (inv_low < inv_cap && inv_pos[inv_low]) inv_low++;
}

seq_t a(idx_t n)
{
    uint64_t t = (uint64_t)n;
    for (;;) {
        if (t < inv_cap && inv_pos[t]) break;
        inv_step();
    }
    return (seq_t)(inv_pos[t] - 1);
}
