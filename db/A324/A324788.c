/*
 * A324788  Value of A022837 at its n-th low point.
 * https://oeis.org/A324788
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = A022837(k_n), where k_n is the n-th (n>=0) index k with
 *            A022837(k) < prime(k+1) and A022837(k)+prime(k+1) < prime(k+2)
 *            (two consecutive "+" steps, a low point); found by scanning
 *            A022837
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 11, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Validated: partial, pass, 12 terms through n = 11 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

static uint64_t *lp_primes = NULL;   /* lp_primes[0]=1, lp_primes[i]=prime(i) */
static size_t lp_plen = 0;
static size_t lp_pcap = 0;
static uint64_t lp_cand = 3;

static void lp_push_prime(uint64_t v)
{
    if (lp_plen == lp_pcap) {
        size_t nc = lp_pcap ? lp_pcap * 2 : 1024;
        uint64_t *t = (uint64_t *)realloc(lp_primes, nc * sizeof(uint64_t));
        if (!t) abort();
        lp_primes = t;
        lp_pcap = nc;
    }
    lp_primes[lp_plen++] = v;
}

static void lp_ensure_primes(size_t idx)
{
    if (lp_plen == 0) {
        lp_push_prime(1);
        lp_push_prime(2);
        lp_cand = 3;
    }
    while (lp_plen <= idx) {
        uint64_t c = lp_cand;
        bool ok = true;
        for (size_t j = 1; j < lp_plen; ++j) {
            uint64_t p = lp_primes[j];
            if (p * p > c) break;
            if (c % p == 0) { ok = false; break; }
        }
        if (ok) lp_push_prime(c);
        lp_cand += 2;
    }
}

static seq_t *lp_vals = NULL;
static size_t lp_len = 0;
static size_t lp_cap = 0;
static size_t lp_k = 0;
static seq_t lp_a = 1;
static bool lp_init = false;

seq_t a(idx_t n)
{
    if (!lp_init) {
        lp_init = true;
        lp_k = 0;
        lp_a = 1;
    }
    size_t sn = (size_t)n;
    while (lp_len <= sn) {
        lp_ensure_primes(lp_k + 2);
        seq_t p1 = (seq_t)lp_primes[lp_k + 1];
        seq_t p2 = (seq_t)lp_primes[lp_k + 2];
        bool up = (lp_a < p1);
        if (up && (lp_a + p1 < p2)) {
            if (lp_len == lp_cap) {
                size_t nc = lp_cap ? lp_cap * 2 : 64;
                seq_t *t = mem_resize(seq_t, lp_vals, nc);
                if (!t) abort();
                lp_vals = t;
                lp_cap = nc;
            }
            lp_vals[lp_len++] = lp_a;
        }
        if (up) lp_a = lp_a + p1;
        else lp_a = lp_a - p1;
        ++lp_k;
    }
    return lp_vals[sn];
}
