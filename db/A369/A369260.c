/*
 * A369260  Lexicographically earliest infinite sequence such that a(i) = a(j)
 *          => A342671(i) = A342671(j) and A349162(i) = A349162(j), for all i,
 *          j >= 1.
 * https://oeis.org/A369260
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = restricted growth sequence transform of the pairs
 *            [sigma(n), A342671(n)], where A342671(n) = gcd(sigma(n),
 *            A003961(n)); computed by scanning n = 1.. and numbering distinct
 *            pairs by first occurrence
 * Range:     u8 n <= 55, u16 n <= 2664, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 110 terms through n = 9863 (2026-11-10)
 * Helpers:   gcd, ipow_idx, mem, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/mem.h"
#include "../utils/gcd.h"
#include "../utils/next_prime.h"
#include "../utils/ipow_idx.h"

static void a369260_keys(uint64_t n, seq_t *sg, seq_t *g)
{
    seq_t sigma = 1, a3 = 1;
    uint64_t m = n;
    uint64_t p;
    for (p = 2; p * p <= m; p += (p == 2 ? 1 : 2)) {
        if (m % p == 0) {
            seq_t term = 1, pw = 1;
            uint64_t e = 0;
            while (m % p == 0) {
                m /= p;
                pw *= (seq_t)p;
                term += pw;
                e++;
            }
            sigma *= term;
            a3 *= ipow_idx((seq_t)next_prime(p), (idx_t)e);
        }
    }
    if (m > 1) {
        sigma *= ((seq_t)m + 1);
        a3 *= (seq_t)next_prime(m);
    }
    *sg = sigma;
    *g = gcd(sigma, a3);
}

static uint32_t *a369260_ids = NULL;
static seq_t *a369260_dsig = NULL;
static seq_t *a369260_dg = NULL;
static size_t a369260_cap = 0;
static size_t a369260_nd = 0;
static uint64_t a369260_done = 0;

seq_t a(idx_t n)
{
    if (a369260_cap == 0) {
        a369260_cap = 1024;
        a369260_ids = mem_new(uint32_t, a369260_cap + 1);
        a369260_dsig = mem_new(seq_t, a369260_cap + 1);
        a369260_dg = mem_new(seq_t, a369260_cap + 1);
    }
    if ((uint64_t)n > a369260_cap) {
        size_t nc = a369260_cap;
        while ((uint64_t)n > nc) nc *= 2;
        a369260_ids = mem_resize(uint32_t, a369260_ids, nc + 1);
        a369260_dsig = mem_resize(seq_t, a369260_dsig, nc + 1);
        a369260_dg = mem_resize(seq_t, a369260_dg, nc + 1);
        a369260_cap = nc;
    }
    while (a369260_done < (uint64_t)n) {
        uint64_t m = a369260_done + 1;
        seq_t s, g;
        a369260_keys(m, &s, &g);
        size_t j;
        bool found = false;
        for (j = 0; j < a369260_nd; j++) {
            if (a369260_dsig[j] == s && a369260_dg[j] == g) { found = true; break; }
        }
        if (!found) {
            j = a369260_nd++;
            a369260_dsig[j] = s;
            a369260_dg[j] = g;
        }
        a369260_ids[m] = (uint32_t)(j + 1);
        a369260_done = m;
    }
    return (seq_t)a369260_ids[n];
}
