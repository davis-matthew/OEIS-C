/*
 * A387072  Index of n in A386482, or -1 if n does not appear in A386482.
 * https://oeis.org/A387072
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = index of n in A386482 (simulation: A386482 is generated
 *            term by term until n appears; the greatest unused k < a(m-1)
 *            sharing a factor is found per prime factor using union-find
 *            "next unused multiple" structures; return -1 if n is not found
 *            within the search limit)
 * Range:     u8 n <= 40, u16 n <= 918, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 82 terms through n = 9724 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define A387072_NSP 6
static const uint32_t A387072_sp[A387072_NSP] = {2, 3, 5, 7, 11, 13};
static uint32_t *A387072_dsu[A387072_NSP] = {0, 0, 0, 0, 0, 0};

static uint32_t *A387072_terms = NULL;   /* A386482 terms, 0-based */
static size_t A387072_len = 0, A387072_tcap = 0;

static uint8_t *A387072_used = NULL;     /* values 0..vcap-1 */
static uint32_t *A387072_spf = NULL;
static size_t A387072_vcap = 0;

static uint32_t *A387072_pos = NULL;     /* pos[v] = 1-based index of v in A386482, 0 if unseen */
static size_t A387072_pcap = 0;

#define A387072_LIMIT 40000000ULL

static void A387072_grow_values(size_t need)
{
    if (need < A387072_vcap) return;
    size_t nc = A387072_vcap ? A387072_vcap : ((size_t)1 << 16);
    while (nc <= need) nc <<= 1;
    size_t oc = A387072_vcap;

    uint8_t *u = (uint8_t *)realloc(A387072_used, nc);
    if (!u) abort();
    memset(u + oc, 0, nc - oc);
    A387072_used = u;

    uint32_t *s = (uint32_t *)realloc(A387072_spf, nc * sizeof(uint32_t));
    if (!s) abort();
    memset(s, 0, nc * sizeof(uint32_t));
    for (size_t i = 2; i < nc; i++) {
        if (s[i] == 0) {
            s[i] = (uint32_t)i;
            if ((uint64_t)i * i < (uint64_t)nc)
                for (size_t j = i * i; j < nc; j += i)
                    if (s[j] == 0) s[j] = (uint32_t)i;
        }
    }
    A387072_spf = s;

    for (int k = 0; k < A387072_NSP; k++) {
        size_t p = A387072_sp[k];
        size_t os = oc ? oc / p + 1 : 0;
        size_t ns = nc / p + 1;
        uint32_t *d = (uint32_t *)realloc(A387072_dsu[k], ns * sizeof(uint32_t));
        if (!d) abort();
        for (size_t j = os; j < ns; j++) d[j] = (uint32_t)j;
        A387072_dsu[k] = d;
    }
    A387072_vcap = nc;
}

static uint32_t A387072_find(uint32_t *arr, uint32_t j)
{
    uint32_t r = j;
    while (arr[r] != r) r = arr[r];
    while (arr[j] != r) { uint32_t nx = arr[j]; arr[j] = r; j = nx; }
    return r;
}

static void A387072_push(uint32_t v)
{
    if (A387072_len >= A387072_tcap) {
        size_t nc = A387072_tcap ? A387072_tcap * 2 : 4096;
        uint32_t *t = (uint32_t *)realloc(A387072_terms, nc * sizeof(uint32_t));
        if (!t) abort();
        A387072_terms = t;
        A387072_tcap = nc;
    }
    A387072_grow_values((size_t)v);
    A387072_terms[A387072_len++] = v;
    A387072_used[v] = 1;
    for (int k = 0; k < A387072_NSP; k++) {
        uint32_t p = A387072_sp[k];
        if (v % p == 0) A387072_dsu[k][v / p] = v / p - 1;
    }
    if ((size_t)v < A387072_pcap && A387072_pos[v] == 0)
        A387072_pos[v] = (uint32_t)A387072_len;
}

static void A387072_step(void)
{
    uint32_t prev = A387072_terms[A387072_len - 1];
    uint32_t pr[16];
    int np = 0;
    uint32_t x = prev;
    while (x > 1) {
        uint32_t p = A387072_spf[x];
        pr[np++] = p;
        while (x % p == 0) x /= p;
    }

    uint64_t best = 0;
    for (int i = 0; i < np; i++) {
        uint32_t p = pr[i];
        uint32_t jmax = (prev - 1) / p;
        if (jmax == 0) continue;
        int si = -1;
        for (int k = 0; k < A387072_NSP; k++)
            if (A387072_sp[k] == p) { si = k; break; }
        if (si >= 0) {
            uint32_t j = A387072_find(A387072_dsu[si], jmax);
            if (j > 0 && (uint64_t)j * p > best) best = (uint64_t)j * p;
        } else {
            uint32_t j = jmax;
            while (j > 0 && (uint64_t)j * p > best && A387072_used[(size_t)j * p]) j--;
            if (j > 0 && (uint64_t)j * p > best) best = (uint64_t)j * p;
        }
    }

    if (best == 0) {
        best = UINT64_MAX;
        for (int i = 0; i < np; i++) {
            uint64_t p = pr[i];
            uint64_t j = (uint64_t)prev / p + 1;
            for (;;) {
                uint64_t v = j * p;
                if (v >= best) break;
                A387072_grow_values((size_t)v);
                if (!A387072_used[v]) { best = v; break; }
                j++;
            }
        }
    }
    A387072_push((uint32_t)best);
}

seq_t a(idx_t n)
{
    seq_t neg1 = (seq_t)0 - (seq_t)1;
    if (n == 0) return neg1;
    if (n > (idx_t)((uint64_t)1 << 28)) return neg1;

    if (!A387072_terms) {
        A387072_grow_values(1024);
        A387072_push(1);
        A387072_push(2);
    }

    size_t target = (size_t)n;
    if (target >= A387072_pcap) {
        size_t nc = A387072_pcap ? A387072_pcap : 1024;
        while (nc <= target) nc <<= 1;
        uint32_t *p = (uint32_t *)realloc(A387072_pos, nc * sizeof(uint32_t));
        if (!p) abort();
        memset(p, 0, nc * sizeof(uint32_t));
        A387072_pos = p;
        A387072_pcap = nc;
        for (size_t i = 0; i < A387072_len; i++) {
            uint32_t v = A387072_terms[i];
            if ((size_t)v < nc && p[v] == 0) p[v] = (uint32_t)(i + 1);
        }
    }

    while (A387072_pos[target] == 0) {
        if ((unsigned long long)A387072_len >= A387072_LIMIT) return neg1;
        A387072_step();
    }
    return (seq_t)A387072_pos[target];
}
