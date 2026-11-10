/*
 * A267374  Positions of 1's in A267371.
 * https://oeis.org/A267374
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = position of the n-th 1 in A267371 (start with "01",
 *            repeatedly append the shortest prefix that occurs exactly once;
 *            computed by simulation using the Z-function)
 * Range:     u8 n <= 90, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 86 terms through n = 9975 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

static unsigned char *A267374_S = NULL;
static size_t A267374_len = 0, A267374_cap = 0;
static size_t *A267374_pos = NULL;
static size_t A267374_np = 0, A267374_pcap = 0, A267374_scanned = 0;

static void A267374_reserve(size_t need)
{
    if (need <= A267374_cap) return;
    size_t nc = A267374_cap ? A267374_cap : 64;
    while (nc < need) nc <<= 1;
    unsigned char *t = (unsigned char *)realloc(A267374_S, nc);
    if (!t) abort();
    A267374_S = t;
    A267374_cap = nc;
}

static void A267374_scan(void)
{
    for (; A267374_scanned < A267374_len; ++A267374_scanned) {
        if (A267374_S[A267374_scanned]) {
            if (A267374_np == A267374_pcap) {
                size_t nc = A267374_pcap ? A267374_pcap * 2 : 64;
                size_t *t = (size_t *)realloc(A267374_pos, nc * sizeof(size_t));
                if (!t) abort();
                A267374_pos = t;
                A267374_pcap = nc;
            }
            A267374_pos[A267374_np++] = A267374_scanned + 1;
        }
    }
}

/* one growth step: append shortest uniquely-occurring prefix */
static void A267374_step(void)
{
    size_t n = A267374_len;
    const unsigned char *s = A267374_S;
    size_t *z = (size_t *)malloc(n * sizeof(size_t));
    if (!z) abort();
    size_t M = 0, l = 0, r = 0;
    z[0] = n;
    for (size_t i = 1; i < n; ++i) {
        size_t k = 0;
        if (i < r) {
            k = z[i - l];
            if (k > r - i) k = r - i;
        }
        while (i + k < n && s[k] == s[i + k]) ++k;
        z[i] = k;
        if (i + k > r) { l = i; r = i + k; }
        if (k > M) M = k;
    }
    free(z);
    size_t L = M + 1;
    if (L > n) L = n;
    A267374_reserve(n + L);
    memcpy(A267374_S + n, A267374_S, L);
    A267374_len = n + L;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (A267374_len == 0) {
        A267374_reserve(2);
        A267374_S[0] = 0;
        A267374_S[1] = 1;
        A267374_len = 2;
        A267374_scan();
    }
    while (A267374_np < (size_t)n) {
        A267374_step();
        A267374_scan();
    }
    return (seq_t)A267374_pos[(size_t)n - 1];
}
