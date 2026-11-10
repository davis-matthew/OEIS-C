/*
 * A304048  Numbers k such that A022567(k) is divisible by k.
 * https://oeis.org/A304048
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   search — k is a term iff A022567(k) == 0 (mod k), where
 *            A022567(k) = Sum_{T_j = j*(j+1)/2 <= k} p(k - T_j). The
 *            partition numbers p(m) are computed exactly with Euler's
 *            pentagonal recurrence p(m) = Sum_{j>=1} (-1)^(j+1) (p(m -
 *            j(3j-1)/2) + p(m - j(3j+1)/2)), p(0) = 1
 * Range:     u8 n <= 7, u16 n <= 21, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Validated: partial, pass, 26 terms through n = 26 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdbool.h>

/* Exact partition numbers stored as 32-bit limbs, least significant limb first */
static uint32_t *pd = NULL;
static size_t pd_len = 0, pd_cap = 0;
static size_t *po = NULL;          /* po[i] = start of p(i); po[pn] = end */
static size_t pn = 0, po_cap = 0;

static int64_t *acc = NULL;        /* signed per-limb carry-save accumulator */
static uint32_t *tmp = NULL;
static size_t acc_cap = 0;

static size_t plen(size_t i) { return po[i + 1] - po[i]; }

static void ensure_acc(size_t L)
{
    if (L > acc_cap) {
        size_t c = acc_cap ? acc_cap : 64;
        while (c < L) c <<= 1;
        acc = (int64_t *)realloc(acc, c * sizeof(int64_t));
        tmp = (uint32_t *)realloc(tmp, c * sizeof(uint32_t));
        acc_cap = c;
    }
}

static void append_p(const uint32_t *x, size_t len)
{
    if (pd_len + len > pd_cap) {
        size_t c = pd_cap ? pd_cap : 4096;
        while (c < pd_len + len) c <<= 1;
        pd = (uint32_t *)realloc(pd, c * sizeof(uint32_t));
        pd_cap = c;
    }
    if (pn + 2 > po_cap) {
        size_t c = po_cap ? po_cap : 4096;
        while (c < pn + 2) c <<= 1;
        po = (size_t *)realloc(po, c * sizeof(size_t));
        po_cap = c;
    }
    if (pn == 0) po[0] = 0;
    memcpy(pd + pd_len, x, len * sizeof(uint32_t));
    pd_len += len;
    pn++;
    po[pn] = pd_len;
}

static void addv(int64_t *a, const uint32_t *s, size_t len)
{
    for (size_t i = 0; i < len; ++i) a[i] += (int64_t)s[i];
}

static void subv(int64_t *a, const uint32_t *s, size_t len)
{
    for (size_t i = 0; i < len; ++i) a[i] -= (int64_t)s[i];
}

/* Normalize acc[0..L) (value known to be >= 0) into tmp; returns trimmed length */
static size_t normalize(size_t L)
{
    int64_t c = 0;
    for (size_t i = 0; i < L; ++i) {
        int64_t v = acc[i] + c;
        uint32_t limb = (uint32_t)((uint64_t)v & 0xffffffffULL);
        tmp[i] = limb;
        c = (v - (int64_t)limb) / (int64_t)4294967296LL;
    }
    while (L > 1 && tmp[L - 1] == 0) --L;
    return L;
}

static void compute_next_partition(void)
{
    if (pn == 0) {
        uint32_t one = 1;
        append_p(&one, 1);
        return;
    }
    size_t n = pn;
    size_t L = plen(n - 1) + 2;
    ensure_acc(L);
    memset(acc, 0, L * sizeof(int64_t));
    for (size_t j = 1; ; ++j) {
        size_t g1 = j * (3 * j - 1) / 2;
        if (g1 > n) break;
        size_t g2 = g1 + j;
        if (j & 1) {
            addv(acc, pd + po[n - g1], plen(n - g1));
            if (g2 <= n) addv(acc, pd + po[n - g2], plen(n - g2));
        } else {
            subv(acc, pd + po[n - g1], plen(n - g1));
            if (g2 <= n) subv(acc, pd + po[n - g2], plen(n - g2));
        }
    }
    size_t len = normalize(L);
    append_p(tmp, len);
}

/* A022567(k) mod k */
static uint64_t a022567_mod(uint64_t k)
{
    while (pn <= (size_t)k) compute_next_partition();
    size_t L = plen((size_t)k) + 2;
    ensure_acc(L);
    memset(acc, 0, L * sizeof(int64_t));
    for (uint64_t j = 0; ; ++j) {
        uint64_t t = j * (j + 1) / 2;
        if (t > k) break;
        size_t m = (size_t)(k - t);
        addv(acc, pd + po[m], plen(m));
    }
    size_t len = normalize(L);
    unsigned __int128 r = 0;
    for (size_t i = len; i-- > 0; )
        r = ((r << 32) | (unsigned __int128)tmp[i]) % k;
    return (uint64_t)r;
}

static uint64_t *found = NULL;
static size_t found_cnt = 0, found_cap = 0;
static uint64_t next_k = 1;

seq_t a(idx_t n)
{
    while (found_cnt < (size_t)n) {
        uint64_t k = next_k++;
        if (a022567_mod(k) == 0) {
            if (found_cnt == found_cap) {
                found_cap = found_cap ? 2 * found_cap : 64;
                found = (uint64_t *)realloc(found, found_cap * sizeof(uint64_t));
            }
            found[found_cnt++] = k;
        }
    }
    return (seq_t)found[n - 1];
}
