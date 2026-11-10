/*
 * A304045  Numbers k such that A000712(k) is divisible by k.
 * https://oeis.org/A304045
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   search: k is a term iff A000712(k) == 0 (mod k). A000712 is
 *            computed exactly, as multi-limb integers, from p(m) = Sum_{k>=1}
 *            (-1)^(k+1) (p(m-k(3k-1)/2) + p(m-k(3k+1)/2)), p(0) = 1,
 *            A000712(m) = p(m) + Sum_{k>=1} (-1)^(k+1) (A000712(m-k(3k-1)/2)
 *            + A000712(m-k(3k+1)/2)), A000712(0) = 1 (this is pro...
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Validated: partial, pass, 12 terms through n = 12 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define LB 52
#define LMASK ((((uint64_t)1) << LB) - 1)
#define LBASE (((int64_t)1) << LB)

static void *g_grow(void *p, size_t *cap, size_t need, size_t elsz) {
    if (need <= *cap) return p;
    size_t nc = *cap ? *cap : 1024;
    while (nc < need) nc *= 2;
    p = realloc(p, nc * elsz);
    if (!p) abort();
    *cap = nc;
    return p;
}

static int64_t *Pp = 0, *Ap = 0, *Acc = 0;
static size_t Pcap = 0, Acap = 0, Pused = 0, Aused = 0, Acccap = 0;
static size_t *Poff = 0, *Plen = 0, *Aoff = 0, *Alen = 0;
static size_t Icap = 0;
static uint64_t Mnext = 0;
static uint64_t *Terms = 0;
static size_t Nterms = 0, Tcap = 0;

static void ensure_idx(size_t need) {
    if (need <= Icap) return;
    size_t nc = Icap ? Icap : 1024;
    while (nc < need) nc *= 2;
    Poff = (size_t *)realloc(Poff, nc * sizeof(size_t));
    Plen = (size_t *)realloc(Plen, nc * sizeof(size_t));
    Aoff = (size_t *)realloc(Aoff, nc * sizeof(size_t));
    Alen = (size_t *)realloc(Alen, nc * sizeof(size_t));
    if (!Poff || !Plen || !Aoff || !Alen) abort();
    Icap = nc;
}

/* lengths are always padded to multiples of 4 */
static void limb_add(int64_t *__restrict acc, const int64_t *__restrict s, size_t len) {
    for (size_t i = 0; i < len; i += 4) {
        acc[i] += s[i]; acc[i + 1] += s[i + 1];
        acc[i + 2] += s[i + 2]; acc[i + 3] += s[i + 3];
    }
}

static void limb_sub(int64_t *__restrict acc, const int64_t *__restrict s, size_t len) {
    for (size_t i = 0; i < len; i += 4) {
        acc[i] -= s[i]; acc[i + 1] -= s[i + 1];
        acc[i + 2] -= s[i + 2]; acc[i + 3] -= s[i + 3];
    }
}

static void store_norm(int64_t **pool, size_t *cap, size_t *used, size_t *off,
                       size_t *len, uint64_t m, const int64_t *acc, size_t W) {
    *pool = (int64_t *)g_grow(*pool, cap, *used + W + 8, sizeof(int64_t));
    int64_t *out = *pool + *used;
    int64_t carry = 0;
    size_t i;
    for (i = 0; i < W; i++) {
        int64_t v = acc[i] + carry;
        int64_t lo = (int64_t)((uint64_t)v & LMASK);
        carry = (v - lo) / LBASE;
        out[i] = lo;
    }
    while (carry > 0) {
        out[i++] = (int64_t)((uint64_t)carry & LMASK);
        carry >>= LB;
    }
    while (i > 1 && out[i - 1] == 0) i--;
    while (i & 3) out[i++] = 0;
    off[m] = *used;
    len[m] = i;
    *used += i;
}

static void step(uint64_t m) {
    ensure_idx((size_t)m + 1);
    if (m == 0) {
        int64_t one[4] = {1, 0, 0, 0};
        store_norm(&Pp, &Pcap, &Pused, Poff, Plen, 0, one, 4);
        store_norm(&Ap, &Acap, &Aused, Aoff, Alen, 0, one, 4);
        return;
    }
    /* p(m) */
    size_t W = Plen[m - 1];
    Acc = (int64_t *)g_grow(Acc, &Acccap, W + 8, sizeof(int64_t));
    memset(Acc, 0, W * sizeof(int64_t));
    for (uint64_t k = 1;; k++) {
        uint64_t g1 = k * (3 * k - 1) / 2;
        if (g1 > m) break;
        uint64_t g2 = g1 + k;
        if (k & 1) {
            limb_add(Acc, Pp + Poff[m - g1], Plen[m - g1]);
            if (g2 <= m) limb_add(Acc, Pp + Poff[m - g2], Plen[m - g2]);
        } else {
            limb_sub(Acc, Pp + Poff[m - g1], Plen[m - g1]);
            if (g2 <= m) limb_sub(Acc, Pp + Poff[m - g2], Plen[m - g2]);
        }
    }
    store_norm(&Pp, &Pcap, &Pused, Poff, Plen, m, Acc, W);

    /* A000712(m) */
    W = Alen[m - 1];
    if (Plen[m] > W) W = Plen[m];
    Acc = (int64_t *)g_grow(Acc, &Acccap, W + 8, sizeof(int64_t));
    memset(Acc, 0, W * sizeof(int64_t));
    limb_add(Acc, Pp + Poff[m], Plen[m]);
    for (uint64_t k = 1;; k++) {
        uint64_t g1 = k * (3 * k - 1) / 2;
        if (g1 > m) break;
        uint64_t g2 = g1 + k;
        if (k & 1) {
            limb_add(Acc, Ap + Aoff[m - g1], Alen[m - g1]);
            if (g2 <= m) limb_add(Acc, Ap + Aoff[m - g2], Alen[m - g2]);
        } else {
            limb_sub(Acc, Ap + Aoff[m - g1], Alen[m - g1]);
            if (g2 <= m) limb_sub(Acc, Ap + Aoff[m - g2], Alen[m - g2]);
        }
    }
    store_norm(&Ap, &Acap, &Aused, Aoff, Alen, m, Acc, W);

    /* test A000712(m) mod m */
    const int64_t *x = Ap + Aoff[m];
    uint64_t r = 0;
    for (size_t i = Alen[m]; i-- > 0;) {
        uint64_t l = (uint64_t)x[i];
        r = ((r << 26) | (l >> 26)) % m;
        r = ((r << 26) | (l & 0x3FFFFFFu)) % m;
    }
    if (r == 0) {
        Terms = (uint64_t *)g_grow(Terms, &Tcap, Nterms + 1, sizeof(uint64_t));
        Terms[Nterms++] = m;
    }
}

seq_t a(idx_t n) {
    while (Nterms < (size_t)n) step(Mnext++);
    return (seq_t)Terms[n - 1];
}
