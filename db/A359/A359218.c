/*
 * A359218  Let S(n) be the sequence obtained through the mapping of
 *          x->A359194(x) starting with n and stopping when 0 is reached, -1
 *          if 0 is never reached. a(n) = m if appears in S(k), k < n,
 *          otherwise -1.
 * https://oeis.org/A359218
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   search. S(n) = iterates of x -> A359194(x) = (binary complement
 *            of 3x), starting at n; a(n) = first element of S(n) that already
 *            occurred in some S(k), k < n; a(0) = 0. Trajectories are
 *            followed in multi-limb (arbitrary length) arithmetic; seen
 *            values are kept in a hash set of 64-bit fingerprints
 * Range:     u8 n <= 170, u16 n <= 1396, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 86 terms through n = 9858 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/mem.h"

/* ---------- multi-limb current value ---------- */
static uint64_t *xb = NULL;
static size_t xcap = 0, xlen = 0;

static void x_reserve(size_t need)
{
    if (need <= xcap) return;
    size_t nc = xcap ? xcap : 16;
    while (nc < need) nc *= 2;
    xb = (uint64_t *)realloc(xb, nc * sizeof(uint64_t));
    xcap = nc;
}

/* x -> binary complement of 3x */
static void x_step(void)
{
    x_reserve(xlen + 2);
    if (xlen == 0) { xb[0] = 1; xlen = 1; return; }
    uint64_t carry = 0;
    for (size_t i = 0; i < xlen; i++) {
        unsigned __int128 p = (unsigned __int128)xb[i] * 3u + carry;
        xb[i] = (uint64_t)p;
        carry = (uint64_t)(p >> 64);
    }
    if (carry) xb[xlen++] = carry;
    uint64_t top = xb[xlen - 1];
    int b = 64 - __builtin_clzll(top);
    uint64_t mask = (b == 64) ? ~(uint64_t)0 : ((((uint64_t)1) << b) - 1);
    for (size_t i = 0; i + 1 < xlen; i++) xb[i] = ~xb[i];
    xb[xlen - 1] = (~top) & mask;
    while (xlen > 0 && xb[xlen - 1] == 0) xlen--;
}

static uint64_t mix64(uint64_t z)
{
    z ^= z >> 30; z *= 0xbf58476d1ce4e5b9ULL;
    z ^= z >> 27; z *= 0x94d049bb133111ebULL;
    z ^= z >> 31;
    return z;
}

static uint64_t x_hash(void)
{
    uint64_t h = 0x9E3779B97F4A7C15ULL ^ (uint64_t)xlen;
    for (size_t i = 0; i < xlen; i++) {
        h = mix64(h ^ xb[i]) + 0x632BE59BD9B4E019ULL;
    }
    h = mix64(h);
    if (h == 0) h = 1;
    return h;
}

/* ---------- hash set of fingerprints ---------- */
static uint64_t *hs = NULL;
static size_t hcap = 0, hcnt = 0;

static void hs_insert_raw(uint64_t key)
{
    size_t i = (size_t)(key & (hcap - 1));
    while (hs[i]) {
        if (hs[i] == key) return;
        i = (i + 1) & (hcap - 1);
    }
    hs[i] = key;
    hcnt++;
}

static void hs_grow(void)
{
    size_t oc = hcap;
    uint64_t *ot = hs;
    hcap = oc ? oc * 2 : ((size_t)1 << 12);
    hs = (uint64_t *)calloc(hcap, sizeof(uint64_t));
    hcnt = 0;
    for (size_t i = 0; i < oc; i++)
        if (ot[i]) hs_insert_raw(ot[i]);
    free(ot);
}

static void hs_add(uint64_t key)
{
    if (hcap == 0 || (hcnt + 1) * 2 > hcap) hs_grow();
    hs_insert_raw(key);
}

static int hs_has(uint64_t key)
{
    if (hcap == 0) return 0;
    size_t i = (size_t)(key & (hcap - 1));
    while (hs[i]) {
        if (hs[i] == key) return 1;
        i = (i + 1) & (hcap - 1);
    }
    return 0;
}

/* ---------- path buffer ---------- */
static uint64_t *pb = NULL;
static size_t pcap = 0;

static seq_t x_to_seq(void)
{
    if (xlen == 0) return (seq_t)0;
    if (xlen == 1) return (seq_t)xb[0];
    seq_t r = 0;
    for (size_t i = xlen; i-- > 0;) {
        for (int s = 0; s < 8; s++) r <<= 8;
        r += (seq_t)xb[i];
    }
    return r;
}

static seq_t *res = NULL;
static size_t res_n = 0, res_cap = 0;

seq_t a(idx_t n)
{
    if (res_n == 0) {
        res_cap = 256;
        res = mem_new(seq_t, res_cap);
        xlen = 0;                 /* value 0 */
        hs_add(x_hash());
        res[0] = 0;
        res_n = 1;
    }
    while (res_n <= (size_t)n) {
        uint64_t k = (uint64_t)res_n;
        x_reserve(2);
        if (k == 0) xlen = 0; else { xb[0] = k; xlen = 1; }
        size_t plen = 0;
        seq_t ans = 0;
        uint64_t saved = 0;
        size_t power = 1, lam = 0;
        int first = 1;
        for (;;) {
            uint64_t h = x_hash();
            if (hs_has(h)) { ans = x_to_seq(); break; }
            if (!first && h == saved) { ans = (seq_t)0 - (seq_t)1; break; }
            if (first) { saved = h; first = 0; }
            else if (++lam == power) { saved = h; power *= 2; lam = 0; }
            if (plen >= pcap) {
                pcap = pcap ? pcap * 2 : 4096;
                pb = (uint64_t *)realloc(pb, pcap * sizeof(uint64_t));
            }
            pb[plen++] = h;
            x_step();
        }
        for (size_t j = 0; j < plen; j++) hs_add(pb[j]);
        if (res_n >= res_cap) {
            res_cap *= 2;
            res = mem_resize(seq_t, res, res_cap);
        }
        res[res_n++] = ans;
    }
    return res[(size_t)n];
}
