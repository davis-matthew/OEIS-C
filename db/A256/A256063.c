/*
 * A256063  First differences of A256062.
 * https://oeis.org/A256063
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A256062(n+1) - A256062(n); A256062 built by greedy search
 * Range:     u8 n <= 11, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Validated: partial, pass, 70 terms through n = 70 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"

static seq_t *A256062_seq = NULL;
static size_t A256062_len = 0;
static size_t A256062_cap = 0;

static uint64_t *A256062_bits = NULL;
static size_t A256062_words = 0;

#define A256062_MAXPC 20000

typedef struct { uint64_t word; uint64_t mask; } A256062_Changed;
static A256062_Changed A256062_changed[A256062_MAXPC];
static size_t A256062_changed_cnt = 0;

static void A256062_ensure_seq_cap(size_t cap)
{
    if (cap <= A256062_cap) return;
    size_t new_cap = A256062_cap ? A256062_cap << 1 : 256;
    if (new_cap < cap) new_cap = cap;
    seq_t *ns = mem_new(seq_t, new_cap);
    if (A256062_seq)
    {
        mem_copy(ns, A256062_seq, A256062_len * sizeof(seq_t));
        mem_free(A256062_seq);
    }
    A256062_seq = ns;
    A256062_cap = new_cap;
}

static void A256062_ensure_diff_cap(uint64_t word)
{
    if (word < A256062_words) return;
    size_t nw = A256062_words ? A256062_words << 1 : 1024;
    while (nw <= word) nw <<= 1;
    uint64_t *nb = (uint64_t *)calloc(nw, sizeof(uint64_t));
    if (A256062_bits)
    {
        memcpy(nb, A256062_bits, A256062_words * sizeof(uint64_t));
        free(A256062_bits);
    }
    A256062_bits = nb;
    A256062_words = nw;
}

static seq_t A256062_a(idx_t n)
{
    if (n == 0) return 0;

    if (A256062_len == 0)
    {
        A256062_ensure_seq_cap(1);
        A256062_seq[0] = (seq_t)1;
        A256062_len = 1;
    }

    while (A256062_len < n)
    {
        seq_t last = A256062_seq[A256062_len - 1];
        seq_t cand = last + (seq_t)1;

        for (;;)
        {
            bool ok = true;

            for (size_t i = 1; i < A256062_len; ++i)
            {
                if (cand % A256062_seq[i] == 0)
                {
                    ok = false;
                    break;
                }
            }
            if (!ok)
            {
                cand = cand + (seq_t)1;
                continue;
            }

            A256062_changed_cnt = 0;
            for (size_t i = 0; i < A256062_len; ++i)
            {
                seq_t d = cand - A256062_seq[i];
                uint64_t du = (uint64_t)d;
                uint64_t word = du >> 6;
                uint64_t mask = (uint64_t)1 << (du & 63);
                A256062_ensure_diff_cap(word);
                if (A256062_bits[word] & mask)
                {
                    ok = false;
                    break;
                }
                A256062_bits[word] |= mask;
                A256062_changed[A256062_changed_cnt].word = word;
                A256062_changed[A256062_changed_cnt].mask = mask;
                ++A256062_changed_cnt;
            }

            if (!ok)
            {
                for (size_t j = 0; j < A256062_changed_cnt; ++j)
                {
                    uint64_t w = A256062_changed[j].word;
                    A256062_bits[w] ^= A256062_changed[j].mask;
                }
                cand = cand + (seq_t)1;
                continue;
            }
            break;
        }

        A256062_ensure_seq_cap(A256062_len + 1);
        A256062_seq[A256062_len] = cand;
        ++A256062_len;
    }

    return A256062_seq[n - 1];
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t hi = A256062_a(n + 1);
    seq_t lo = A256062_a(n);
    return hi - lo;
}
