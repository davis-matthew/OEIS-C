/*
 * A261396  a(n) = smallest k such that A260273(k) >= 2^n.
 * https://oeis.org/A261396
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + #{k : A260273(k) < 2^n}. The code steps through
 *            A260273 (x -> x + A261461(x)) using a recursive split over the
 *            binary prefix of x. Each block's result depends only on the
 *            prefix's short substrings and tail, so blocks are memoized with
 *            a key cut at the largest increment used
 * Range:     u8 n <= 10, u16 n <= 19, u32 n <= 35, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Validated: partial, pass, 39 terms through n = 38 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

static uint64_t A261396_Wlo[8192];
static uint64_t A261396_Whi[8192];
static bool A261396_ready = false;
static uint64_t A261396_cache[64];

typedef struct { uint64_t lo, hi, pk; } A261396_ent;
#define A261396_TBITS 22
static A261396_ent *A261396_tab = NULL;
static uint64_t A261396_Tlo[64], A261396_Thi[64];

static unsigned A261396_ctz(uint64_t v)
{
#if defined(__GNUC__) || defined(__clang__)
    return (unsigned)__builtin_ctzll(v);
#else
    unsigned c = 0;
    while (!(v & 1ULL)) { v >>= 1; c++; }
    return c;
#endif
}

static void A261396_build(void)
{
    for (unsigned t = 0; t < 64; t++) {
        for (unsigned r = 0; r < 128; r++) {
            uint64_t lo = 1ULL, hi = 0ULL;
            unsigned comb = (t << 7) | r;
            for (unsigned j = 0; j < 7; j++) {
                for (unsigned l = 1; l <= 7; l++) {
                    unsigned w = (comb >> j) & ((1u << l) - 1u);
                    if (!((w >> (l - 1)) & 1u)) continue;
                    if (w < 64) lo |= 1ULL << w;
                    else hi |= 1ULL << (w - 64);
                }
            }
            A261396_Wlo[(t << 7) | r] = lo;
            A261396_Whi[(t << 7) | r] = hi;
        }
    }
    A261396_tab = (A261396_ent *)calloc((size_t)1 << A261396_TBITS, sizeof(A261396_ent));
    A261396_ready = true;
}

static void A261396_ext(uint64_t Q, uint64_t *lo, uint64_t *hi)
{
    for (unsigned l = 1; l <= 7; l++) {
        uint64_t w = Q & ((1ULL << l) - 1ULL);
        if ((w >> (l - 1)) & 1ULL) {
            if (w < 64) *lo |= 1ULL << w;
            else *hi |= 1ULL << (w - 64);
        }
    }
}

static void A261396_key(uint64_t P, uint64_t mlo, uint64_t mhi, unsigned s,
                        unsigned r0, unsigned T, uint64_t *klo, uint64_t *khi,
                        uint64_t *kb)
{
    unsigned L = 0, t = T;
    while (t) { L++; t >>= 1; }
    uint64_t tl = (P & 63ULL) & ((1ULL << (L - 1)) - 1ULL);
    if (T < 63) { *klo = mlo & ((2ULL << T) - 1ULL); *khi = 0; }
    else if (T == 63) { *klo = mlo; *khi = 0; }
    else {
        *klo = mlo;
        *khi = (T == 127) ? mhi : (mhi & ((2ULL << (T - 64)) - 1ULL));
    }
    *kb = (uint64_t)s | ((uint64_t)r0 << 5) | ((uint64_t)T << 12) | (tl << 19);
}

static uint64_t A261396_hash(uint64_t klo, uint64_t khi, uint64_t kb)
{
    uint64_t h = klo * 0x9E3779B97F4A7C15ULL;
    h ^= (khi + 0x632BE59BD9B4E019ULL) * 0xC2B2AE3D27D4EB4FULL;
    h ^= kb * 0xD6E8FEB86659FD93ULL;
    h ^= h >> 31;
    h *= 0x94D049BB133111EBULL;
    h ^= h >> 29;
    return h;
}

static bool A261396_probe(uint64_t P, uint64_t mlo, uint64_t mhi, unsigned s,
                          unsigned r0, unsigned T, uint64_t *st, unsigned *exitr)
{
    uint64_t klo, khi, kb;
    A261396_key(P, mlo, mhi, s, r0, T, &klo, &khi, &kb);
    uint64_t h = A261396_hash(klo, khi, kb);
    uint64_t mask = ((uint64_t)1 << A261396_TBITS) - 1ULL;
    for (unsigned j = 0; j < 4; j++) {
        A261396_ent *e = &A261396_tab[(h + j) & mask];
        if (e->pk == 0) return false;
        if (e->lo == klo && e->hi == khi && (e->pk & 0x1FFFFFFULL) == kb) {
            *st = e->pk >> 32;
            *exitr = (unsigned)((e->pk >> 25) & 127ULL);
            return true;
        }
    }
    return false;
}

static bool A261396_lookup(uint64_t P, uint64_t mlo, uint64_t mhi, unsigned s,
                           unsigned r0, uint64_t *st, unsigned *exitr, unsigned *am)
{
    uint64_t clo = ~mlo & A261396_Tlo[s], chi = ~mhi & A261396_Thi[s];
    unsigned tries = 0;
    while (clo && tries < 16) {
        unsigned T = A261396_ctz(clo);
        clo &= clo - 1ULL;
        tries++;
        if (A261396_probe(P, mlo, mhi, s, r0, T, st, exitr)) { *am = T; return true; }
    }
    while (chi && tries < 16) {
        unsigned T = 64 + A261396_ctz(chi);
        chi &= chi - 1ULL;
        tries++;
        if (A261396_probe(P, mlo, mhi, s, r0, T, st, exitr)) { *am = T; return true; }
    }
    return false;
}

static void A261396_store(uint64_t P, uint64_t mlo, uint64_t mhi, unsigned s,
                          unsigned r0, unsigned T, uint64_t st, unsigned exitr)
{
    uint64_t klo, khi, kb;
    A261396_key(P, mlo, mhi, s, r0, T, &klo, &khi, &kb);
    uint64_t h = A261396_hash(klo, khi, kb);
    uint64_t mask = ((uint64_t)1 << A261396_TBITS) - 1ULL;
    uint64_t pk = kb | ((uint64_t)exitr << 25) | (st << 32);
    A261396_ent *slot = NULL;
    for (unsigned j = 0; j < 4; j++) {
        A261396_ent *e = &A261396_tab[(h + j) & mask];
        if (e->pk == 0 ||
            (e->lo == klo && e->hi == khi && (e->pk & 0x1FFFFFFULL) == kb)) {
            slot = e;
            break;
        }
    }
    if (!slot) slot = &A261396_tab[(h + (h >> 62)) & mask];
    slot->lo = klo; slot->hi = khi; slot->pk = pk;
    if (T < 64) A261396_Tlo[s] |= 1ULL << T;
    else A261396_Thi[s] |= 1ULL << (T - 64);
}

/* Simulate x = P*2^s + r from r = r0 until r >= 2^s.  mlo/mhi: patterns in P.
   Returns step count; *exitr = overflow; *amax = largest increment used. */
static uint64_t A261396_run(uint64_t P, uint64_t mlo, uint64_t mhi, unsigned s,
                            unsigned r0, unsigned *exitr, unsigned *amax)
{
    uint64_t st = 0;
    unsigned am = 0;
    bool memo = (A261396_tab != NULL && s >= 10 && s <= 31);
    if (memo && A261396_lookup(P, mlo, mhi, s, r0, &st, exitr, &am)) {
        *amax = am;
    } else {
        if (s == 7) {
            unsigned tail = (unsigned)(P & 63ULL) << 7;
            const uint64_t *wl = A261396_Wlo + tail;
            const uint64_t *wh = A261396_Whi + tail;
            unsigned r = r0;
            while (r < 128) {
                uint64_t m = ~(mlo | wl[r]);
                unsigned d;
                if (m) d = A261396_ctz(m);
                else {
                    uint64_t hh = ~(mhi | wh[r]);
                    d = hh ? 64 + A261396_ctz(hh) : 128;
                }
                if (d > am) am = d;
                r += d;
                st++;
            }
            *exitr = r - 128;
        } else {
            unsigned r = r0, a1 = 0, a2 = 0;
            uint64_t Q0 = P << 1, Q1 = (P << 1) | 1ULL;
            uint64_t l0 = mlo, h0 = mhi, l1 = mlo, h1 = mhi;
            A261396_ext(Q0, &l0, &h0);
            A261396_ext(Q1, &l1, &h1);
            st += A261396_run(Q0, l0, h0, s - 1, r, &r, &a1);
            st += A261396_run(Q1, l1, h1, s - 1, r, &r, &a2);
            am = a1 > a2 ? a1 : a2;
            *exitr = r;
        }
        if (memo && am >= 1 && am <= 127 && *exitr < 128)
            A261396_store(P, mlo, mhi, s, r0, am, st, *exitr);
        *amax = am;
    }
    if (P == 0 && s < 64) A261396_cache[s] = st + 1;
    return st;
}

seq_t a(idx_t n)
{
    if (!A261396_ready) A261396_build();
    if (n < 7) {
        uint64_t x = 1, k = 1, lim = 1ULL << (unsigned)n;
        while (x < lim) {
            x += A261396_ctz(~A261396_Wlo[x]);
            k++;
        }
        return (seq_t)k;
    }
    if (n >= 64) return (seq_t)0;
    if (A261396_cache[n] == 0) {
        unsigned e, am;
        A261396_run(0ULL, 1ULL, 0ULL, (unsigned)n, 1u, &e, &am);
    }
    return (seq_t)A261396_cache[n];
}
