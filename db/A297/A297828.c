/*
 * A297828  Difference sequence of A297997.
 * https://oeis.org/A297828
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A297997(n) - A297997(n-1), where A297997 (offset 0) has
 *            b(0)=3, b(1)=4, b(k) = least positive integer not in {c(j)},
 *            c(0)=1, c(1)=2, c(k) = 2*b(k-1) - b(k-2) + k (k>=2); computed by
 *            a sieve-like incremental marking
 * Range:     u8 n <= 1988, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 102 terms through n = 9906 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/mem.h"

static seq_t *d297828_b = NULL;
static size_t d297828_len = 0, d297828_cap = 0;
static unsigned char *d297828_pres = NULL;
static size_t d297828_pcap = 0;
static size_t d297828_next = 3;

static void d297828_ensure_pres(size_t need)
{
    if (need < d297828_pcap) return;
    size_t nc = d297828_pcap ? d297828_pcap : 64;
    while (nc <= need) nc <<= 1;
    unsigned char *t = (unsigned char *)realloc(d297828_pres, nc);
    if (!t) abort();
    memset(t + d297828_pcap, 0, nc - d297828_pcap);
    d297828_pres = t;
    d297828_pcap = nc;
}

static void d297828_push(seq_t v)
{
    if (d297828_len >= d297828_cap) {
        size_t nc = d297828_cap ? d297828_cap << 1 : 64;
        seq_t *t = mem_resize(seq_t, d297828_b, nc);
        if (!t) abort();
        d297828_b = t;
        d297828_cap = nc;
    }
    d297828_b[d297828_len++] = v;
}

static seq_t d297828_B(idx_t n)
{
    if (d297828_len == 0) {
        d297828_ensure_pres(64);
        d297828_pres[1] = 1;
        d297828_pres[2] = 1;
        d297828_push((seq_t)3);
        d297828_push((seq_t)4);
        d297828_next = 5;
    }
    while (d297828_len <= (size_t)n) {
        size_t k = d297828_len;
        seq_t ck = (seq_t)2 * d297828_b[k - 1] - d297828_b[k - 2] + (seq_t)k;
        size_t idx = (size_t)(unsigned long long)ck;
        d297828_ensure_pres(idx + 1);
        d297828_pres[idx] = 1;
        for (;;) {
            d297828_ensure_pres(d297828_next + 1);
            if (!d297828_pres[d297828_next]) break;
            d297828_next++;
        }
        d297828_push((seq_t)d297828_next);
        d297828_next++;
    }
    return d297828_b[(size_t)n];
}

seq_t a(idx_t n)
{
    return d297828_B(n) - d297828_B(n - 1);
}
