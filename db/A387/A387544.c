/*
 * A387544  Even terms of A386482, halved, in order of occurrence.
 * https://oeis.org/A387544
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A386482(k)/2 where A386482(k) is the n-th even term of
 *            A386482; computed by direct simulation (search) of A386482
 * Range:     u8 n <= 115, u16 n <= 10306, u32 n <= 10306, u64 n <= 10306,
 *            u128 n <= 10306, big any n
 * Validated: standard, pass, 83 terms through n = 9053 (2026-11-10)
 * Helpers:   gcd, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/gcd.h"
#include "../utils/mem.h"

static seq_t *A387544_ev = NULL;
static size_t A387544_ev_len = 0, A387544_ev_cap = 0;
static uint8_t *A387544_used = NULL;
static size_t A387544_used_cap = 0;
static seq_t A387544_prev = 0;
static size_t A387544_cnt = 0;
static bool A387544_init = false;

static void A387544_ensure_used(size_t need)
{
    if (need < A387544_used_cap) return;
    size_t nc = A387544_used_cap ? A387544_used_cap : 64;
    while (nc <= need) nc <<= 1;
    uint8_t *t = (uint8_t *)realloc(A387544_used, nc);
    if (!t) abort();
    memset(t + A387544_used_cap, 0, nc - A387544_used_cap);
    A387544_used = t;
    A387544_used_cap = nc;
}

static void A387544_push_even(seq_t v)
{
    if (A387544_ev_len >= A387544_ev_cap) {
        size_t nc = A387544_ev_cap ? A387544_ev_cap * 2 : 64;
        seq_t *t = mem_resize(seq_t, A387544_ev, nc);
        if (!t) abort();
        A387544_ev = t;
        A387544_ev_cap = nc;
    }
    A387544_ev[A387544_ev_len++] = v;
}

static void A387544_take(seq_t v)
{
    A387544_ensure_used((size_t)v);
    A387544_used[(size_t)v] = 1;
    A387544_prev = v;
    if ((v & 1) == 0) A387544_push_even(v / 2);
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (!A387544_init) {
        A387544_init = true;
        A387544_take(1);
        A387544_take(2);
    }
    while (A387544_ev_len < (size_t)n) {
        seq_t prev = A387544_prev;
        seq_t cand = 0;
        bool found = false;
        for (seq_t down = prev; down > 1;) {
            --down;
            if (down < 2) break;
            if (!A387544_used[(size_t)down] && gcd(prev, down) != 1) {
                cand = down;
                found = true;
                break;
            }
        }
        if (!found) {
            for (seq_t up = prev + 1;; ++up) {
                A387544_ensure_used((size_t)up);
                if (!A387544_used[(size_t)up] && gcd(prev, up) != 1) {
                    cand = up;
                    break;
                }
            }
        }
        A387544_take(cand);
    }
    return A387544_ev[(size_t)n - 1];
}
