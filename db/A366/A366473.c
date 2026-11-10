/*
 * A366473  a(1) = 1; thereafter a(n) = A337136(n) + A337136(n-1).
 * https://oeis.org/A366473
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; a(n) = A337136(n) + A337136(n-1) for n > 1, where
 *            A337136(1)=1, A337136(2)=2, and A337136(n) is the smallest
 *            unused positive number sharing a factor with
 *            A337136(n-2)+A337136(n-1) (computed by search)
 * Range:     u8 n <= 71, u16 n <= 7395, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 99 terms through n = 9784 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/mem.h"

static seq_t *A337136_seq = NULL;
static size_t A337136_cap = 0;
static size_t A337136_len = 0;

static uint8_t *A337136_used = NULL;
static size_t A337136_used_cap = 0;

static void A337136_ensure_used(uint64_t needed)
{
    if (needed < A337136_used_cap)
        return;
    size_t new_cap = A337136_used_cap ? A337136_used_cap << 1 : 1;
    while (new_cap <= needed)
        new_cap <<= 1;
    uint8_t *tmp = (uint8_t *)realloc(A337136_used, new_cap * sizeof(uint8_t));
    if (!tmp)
        abort();
    A337136_used = tmp;
    memset(A337136_used + A337136_used_cap, 0, (new_cap - A337136_used_cap) * sizeof(uint8_t));
    A337136_used_cap = new_cap;
}

static void A337136_ensure_seq(size_t needed)
{
    if (needed <= A337136_cap)
        return;
    size_t new_cap = A337136_cap ? A337136_cap << 1 : 1;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *tmp = mem_resize(seq_t, A337136_seq, new_cap);
    if (!tmp)
        abort();
    A337136_seq = tmp;
    A337136_cap = new_cap;
}

static void A337136_factor_unique(uint64_t n, uint64_t *factors, size_t *cnt)
{
    *cnt = 0;
    for (uint64_t p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            factors[(*cnt)++] = p;
            while (n % p == 0)
                n /= p;
        }
    }
    if (n > 1)
        factors[(*cnt)++] = n;
}

static uint64_t A337136_next_unused_multiple(uint64_t prime)
{
    uint64_t cand = prime;
    while (true) {
        A337136_ensure_used(cand);
        if (!A337136_used[cand])
            return cand;
        cand += prime;
    }
}

static seq_t A337136_a(idx_t n)
{
    if (A337136_len == 0) {
        A337136_ensure_seq(2);
        A337136_seq[0] = (seq_t)1;
        A337136_seq[1] = (seq_t)2;
        A337136_len = 2;
        A337136_ensure_used(2);
        A337136_used[1] = 1;
        A337136_used[2] = 1;
    }
    while (A337136_len < (size_t)n) {
        uint64_t sum = (uint64_t)A337136_seq[A337136_len - 2] + (uint64_t)A337136_seq[A337136_len - 1];
        uint64_t primes[64];
        size_t pc;
        A337136_factor_unique(sum, primes, &pc);
        uint64_t best = UINT64_MAX;
        for (size_t i = 0; i < pc; ++i) {
            uint64_t cand = A337136_next_unused_multiple(primes[i]);
            if (cand < best)
                best = cand;
        }
        A337136_ensure_seq(A337136_len + 1);
        A337136_seq[A337136_len++] = (seq_t)best;
        A337136_ensure_used(best);
        A337136_used[best] = 1;
    }
    return A337136_seq[n - 1];
}

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;
    seq_t x = A337136_a(n);
    seq_t y = A337136_a(n - 1);
    return x + y;
}
