/*
 * A384970  a(n) is the length of the largest run of consecutive terms of
 *          A386482 containing A386482(n) and sharing a common prime factor.
 * https://oeis.org/A384970
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = max over primes p | A386482(n) of the length of the
 *            maximal run of consecutive terms of A386482 divisible by p that
 *            contains A386482(n); a(1)=0 (computed by direct search)
 * Range:     u8 n <= 82, u16 n <= 10117, u32 n <= 10117, u64 n <= 10117,
 *            u128 n <= 10117, big any n
 * Validated: standard, pass, 98 terms through n = 9767 (2026-11-10)
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

static seq_t A386482_a(idx_t n)
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
    if ((size_t)n <= A386482_seq_len)
        return A386482_seq[(size_t)n - 1];

    while (A386482_seq_len < (size_t)n) {
        seq_t prev = A386482_seq[A386482_seq_len - 1];
        seq_t cand = (seq_t)0;
        bool found = false;

        if (prev > (seq_t)1) {
            for (seq_t down = prev - 1; down >= (seq_t)1; --down) {
                size_t idx = (size_t)down;
                A386482_ensure_used_cap(idx);
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
    return A386482_seq[(size_t)n - 1];
}

static idx_t A384970_run(idx_t n, seq_t p)
{
    idx_t lo = n, hi = n;
    while (lo > 1 && A386482_a(lo - 1) % p == 0) lo--;
    while (A386482_a(hi + 1) % p == 0) hi++;
    return hi - lo + 1;
}

seq_t a(idx_t n)
{
    if (n <= 1) return 0;
    seq_t v = A386482_a(n);
    idx_t best = 0;
    for (seq_t p = 2; p * p <= v; p++) {
        if (v % p == 0) {
            while (v % p == 0) v /= p;
            idx_t r = A384970_run(n, p);
            if (r > best) best = r;
        }
    }
    if (v > 1) {
        idx_t r = A384970_run(n, v);
        if (r > best) best = r;
    }
    return (seq_t)best;
}
