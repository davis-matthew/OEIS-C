/*
 * A343878  a(n) is the least k such that A342585(k) = n.
 * https://oeis.org/A343878
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look, easy
 * Formula:   a(n) = least k such that A342585(k) = n; computed by generating
 *            A342585 (inventory sequence) and recording first occurrences
 * Range:     u8 n <= 20, u16 n <= 470, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 70 terms through n = 9123 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static idx_t *A343878_freq = NULL;
static idx_t *A343878_first = NULL;
static size_t A343878_cap = 0;
static idx_t A343878_pos = 0;
static idx_t A343878_si = 0;

static bool A343878_ensure(size_t need)
{
    if (need < A343878_cap) return true;
    size_t nc = A343878_cap ? A343878_cap : 64;
    while (nc <= need) nc <<= 1;
    idx_t *f = (idx_t*)realloc(A343878_freq, sizeof(idx_t) * nc);
    if (!f) return false;
    A343878_freq = f;
    idx_t *g = (idx_t*)realloc(A343878_first, sizeof(idx_t) * nc);
    if (!g) return false;
    A343878_first = g;
    for (size_t i = A343878_cap; i < nc; ++i) { A343878_freq[i] = 0; A343878_first[i] = 0; }
    A343878_cap = nc;
    return true;
}

seq_t a(idx_t n)
{
    if (!A343878_ensure((size_t)n + 1)) return (seq_t)0;
    while (A343878_first[n] == 0) {
        idx_t cnt = (A343878_si < (idx_t)A343878_cap) ? A343878_freq[A343878_si] : 0;
        if (!A343878_ensure((size_t)cnt + 1)) return (seq_t)0;
        ++A343878_pos;
        if (A343878_first[cnt] == 0) A343878_first[cnt] = A343878_pos;
        ++A343878_freq[cnt];
        if (cnt == 0) A343878_si = 0;
        else ++A343878_si;
    }
    return (seq_t)A343878_first[n];
}
