/*
 * A264030  Primes p such that A203069(p) is also prime.
 * https://oeis.org/A264030
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th prime p such that A203069(p) is prime (search over
 *            prime indices p; A203069 generated greedily)
 * Range:     u8 n <= 21, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Validated: partial, pass, 57 terms through n = 57 (2026-11-07)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <stdint.h>
#include "../utils/is_prime_u64.h"
#include "../utils/mem.h"

static seq_t *A203069_seq = NULL;
static size_t A203069_cap = 0;
static size_t A203069_len = 0;
static bool *A203069_used = NULL;
static size_t A203069_used_cap = 0;

static void A203069_ensure_used(size_t idx)
{
    if (idx < A203069_used_cap) return;
    size_t nc = A203069_used_cap ? A203069_used_cap << 1U : 16;
    while (nc <= idx) nc <<= 1U;
    bool *p = (bool *)realloc(A203069_used, nc * sizeof(bool));
    if (!p) abort();
    memset(p + A203069_used_cap, 0, (nc - A203069_used_cap) * sizeof(bool));
    A203069_used = p;
    A203069_used_cap = nc;
}

static void A203069_ensure_seq(size_t len)
{
    if (len <= A203069_cap) return;
    size_t nc = A203069_cap ? A203069_cap << 1U : 16;
    while (nc < len) nc <<= 1U;
    seq_t *p = mem_resize(seq_t, A203069_seq, nc);
    if (!p) abort();
    A203069_seq = p;
    A203069_cap = nc;
}

static seq_t A203069_a(idx_t n)
{
    while (A203069_len < (size_t)n) {
        if (A203069_len == 0) {
            A203069_ensure_seq(1);
            A203069_seq[0] = 1;
            A203069_ensure_used(1);
            A203069_used[1] = true;
            A203069_len = 1;
            continue;
        }
        seq_t prev = A203069_seq[A203069_len - 1];
        bool need_odd = (prev & 1) == 0;
        size_t cand = need_odd ? (size_t)1 : (size_t)2;
        while (true) {
            A203069_ensure_used(cand);
            if (!A203069_used[cand]) {
                seq_t sum = prev + (seq_t)cand;
                if (!is_prime_u64((uint64_t)sum)) {
                    A203069_ensure_seq(A203069_len + 1);
                    A203069_seq[A203069_len++] = (seq_t)cand;
                    A203069_used[cand] = true;
                    break;
                }
            }
            cand += 2;
        }
    }
    return A203069_seq[(size_t)(n - 1)];
}

static bool A264030_small_prime(uint64_t p)
{
    if (p < 2) return false;
    for (uint64_t d = 2; d * d <= p; d++)
        if (p % d == 0) return false;
    return true;
}

static idx_t *A264030_terms = NULL;
static size_t A264030_cnt = 0, A264030_cap = 0;
static uint64_t A264030_next = 2;

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    while (A264030_cnt < (size_t)n) {
        uint64_t p = A264030_next++;
        if (!A264030_small_prime(p)) continue;
        seq_t v = A203069_a((idx_t)p);
        if (!is_prime_u64((uint64_t)v)) continue;
        if (A264030_cnt == A264030_cap) {
            size_t nc = A264030_cap ? A264030_cap * 2 : 16;
            idx_t *t = (idx_t *)realloc(A264030_terms, nc * sizeof(idx_t));
            if (!t) abort();
            A264030_terms = t;
            A264030_cap = nc;
        }
        A264030_terms[A264030_cnt++] = (idx_t)p;
    }
    return (seq_t)A264030_terms[(size_t)(n - 1)];
}
