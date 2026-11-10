/*
 * A259603  a(n) = (A259429(n) + A259429(n+1)) / 2.
 * https://oeis.org/A259603
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (A259429(n) + A259429(n+1))/2, where A259429 is computed
 *            by a greedy search (a(1)=1, then smallest unused odd number
 *            making neighbor mean a cube)
 * Range:     u8 n <= 4, u16 n <= 9934, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 72 terms through n = 9935 (2026-11-07)
 * Helpers:   ipow, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include "../utils/ipow.h"
#include "../utils/mem.h"

static seq_t *A259429_seq     = NULL;
static size_t A259429_len     = 0;
static size_t A259429_cap     = 0;
static uint8_t *A259429_used  = NULL;
static size_t A259429_ubytes  = 0;
static seq_t A259429_maxv     = 0;

static void A259429_ensure(seq_t max_val)
{
    size_t bit_index = (size_t)(max_val >> 1);
    size_t bytes_needed = (bit_index + 1 + 7) >> 3;
    if (bytes_needed > A259429_ubytes) {
        uint8_t *p = (uint8_t *)realloc(A259429_used, bytes_needed);
        if (!p) abort();
        memset(p + A259429_ubytes, 0, bytes_needed - A259429_ubytes);
        A259429_used = p;
        A259429_ubytes = bytes_needed;
    }
}

static void A259429_set(seq_t val)
{
    size_t b = (size_t)(val >> 1);
    A259429_used[b >> 3] |= (uint8_t)(1 << (b & 7));
}

static bool A259429_is_used(seq_t val)
{
    size_t b = (size_t)(val >> 1);
    return (A259429_used[b >> 3] >> (b & 7)) & 1;
}

static void A259429_push(seq_t val)
{
    if (A259429_len == A259429_cap) {
        size_t nc = A259429_cap ? A259429_cap * 2 : 8;
        seq_t *ns = mem_resize(seq_t, A259429_seq, nc);
        if (!ns) abort();
        A259429_seq = ns;
        A259429_cap = nc;
    }
    A259429_seq[A259429_len++] = val;
}

static seq_t A259429_a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    while (A259429_len < n) {
        if (A259429_len == 0) {
            A259429_push((seq_t)1);
            A259429_ensure((seq_t)1);
            A259429_set((seq_t)1);
            A259429_maxv = 1;
            continue;
        }
        seq_t prev = A259429_seq[A259429_len - 1];
        seq_t chosen = 0;
        for (idx_t k = 1; ; ++k) {
            seq_t cube = ipow((seq_t)k, (seq_t)3);
            seq_t twice = cube << 1;
            if (twice <= prev) continue;
            seq_t cand = twice - prev;
            if ((cand > A259429_maxv) || !A259429_is_used(cand)) {
                chosen = cand;
                break;
            }
        }
        A259429_push(chosen);
        A259429_ensure(chosen);
        A259429_set(chosen);
        if (chosen > A259429_maxv) A259429_maxv = chosen;
    }
    return A259429_seq[n - 1];
}

seq_t a(idx_t n)
{
    seq_t x = A259429_a(n);
    seq_t y = A259429_a(n + 1);
    /* both terms are odd, so the sum is even; avoid overflow */
    return (x >> 1) + (y >> 1) + 1;
}
