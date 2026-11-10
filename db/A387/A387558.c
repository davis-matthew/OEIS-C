/*
 * A387558  Numbers k such that A386482(k) is even.
 * https://oeis.org/A387558
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k such that A386482(k) is even (A386482 computed by
 *            direct simulation, then filtered by parity)
 * Range:     u8 n <= 67, u16 n <= 10306, u32 n <= 10306, u64 n <= 10306,
 *            u128 n <= 10306, big any n
 * Validated: standard, pass, 83 terms through n = 8366 (2026-11-10)
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

static seq_t *A386482_seq = NULL;
static size_t A386482_seq_cap = 0;
static size_t A386482_seq_len = 0;

static uint8_t *A386482_used = NULL;
static size_t A386482_used_cap = 0;

static void A386482_ensure_seq_cap(size_t need)
{
    if (need <= A386482_seq_cap) return;
    size_t new_cap = A386482_seq_cap ? A386482_seq_cap : 16;
    while (new_cap < need) new_cap <<= 1;
    seq_t *tmp = mem_resize(seq_t, A386482_seq, new_cap);
    if (!tmp) abort();
    A386482_seq = tmp;
    A386482_seq_cap = new_cap;
}

static void A386482_ensure_used_cap(size_t need)
{
    if (need <= A386482_used_cap) return;
    size_t new_cap = A386482_used_cap ? A386482_used_cap : 16;
    while (new_cap <= need) new_cap <<= 1;
    uint8_t *tmp = (uint8_t *)realloc(A386482_used, new_cap * sizeof(uint8_t));
    if (!tmp) abort();
    memset(tmp + A386482_used_cap, 0, new_cap - A386482_used_cap);
    A386482_used = tmp;
    A386482_used_cap = new_cap;
}

static seq_t A386482_a(size_t n)
{
    if (n == 0) return 0;

    if (!A386482_seq) {
        A386482_ensure_seq_cap(2);
        A386482_seq[0] = (seq_t)1;
        A386482_seq[1] = (seq_t)2;
        A386482_seq_len = 2;
        A386482_ensure_used_cap(2);
        A386482_used[1] = 1;
        A386482_used[2] = 1;
    }

    if (n <= A386482_seq_len)
        return A386482_seq[n - 1];

    while (A386482_seq_len < n) {
        seq_t prev = A386482_seq[A386482_seq_len - 1];
        seq_t cand = (seq_t)0;
        bool found = false;

        if (prev > (seq_t)1) {
            for (seq_t down = prev - 1; down >= (seq_t)1; --down) {
                size_t idx = (size_t)down;
                if (!A386482_used[idx] && gcd(prev, down) != (seq_t)1) {
                    cand = down;
                    found = true;
                    break;
                }
            }
        }

        if (!found) {
            for (seq_t up = prev + 1;; ++up) {
                size_t idx = (size_t)up;
                A386482_ensure_used_cap(idx);
                if (!A386482_used[idx] && gcd(prev, up) != (seq_t)1) {
                    cand = up;
                    break;
                }
            }
        }

        A386482_ensure_seq_cap(A386482_seq_len + 1);
        A386482_seq[A386482_seq_len++] = cand;
        size_t uidx = (size_t)cand;
        A386482_ensure_used_cap(uidx);
        A386482_used[uidx] = 1;
    }

    return A386482_seq[n - 1];
}

static uint64_t *A387558_list = NULL;
static size_t A387558_cap = 0;
static size_t A387558_cnt = 0;
static size_t A387558_next = 1;

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    while (A387558_cnt < (size_t)n) {
        size_t k = A387558_next++;
        seq_t v = A386482_a(k);
        if ((v & 1) == 0) {
            if (A387558_cnt >= A387558_cap) {
                size_t nc = A387558_cap ? A387558_cap * 2 : 64;
                uint64_t *t = (uint64_t *)realloc(A387558_list, nc * sizeof(uint64_t));
                if (!t) abort();
                A387558_list = t;
                A387558_cap = nc;
            }
            A387558_list[A387558_cnt++] = (uint64_t)k;
        }
    }
    return (seq_t)A387558_list[(size_t)n - 1];
}
