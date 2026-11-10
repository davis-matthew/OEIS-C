/*
 * A358425  Numbers k such that A030717(k) = 7.
 * https://oeis.org/A358425
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k with A030717(k) = 7; computed by generating
 *            A030717 (list of frequencies of distinct values) and scanning
 *            for 7's
 * Range:     u8 n <= 5, u16 n <= 314, u32 n <= 4046, u64 n <= 4046,
 *            u128 n <= 4046, big any n
 * Validated: standard, pass, 67 terms through n = 3712 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include "../utils/mem.h"

static size_t *A358425_L = NULL;
static size_t *A358425_F = NULL;
static size_t A358425_len = 0, A358425_cap = 0;
static size_t *A358425_P = NULL;
static size_t A358425_pn = 0, A358425_pcap = 0;
static size_t *A358425_kv = NULL;
static size_t A358425_kvcap = 0;
static size_t A358425_maxv = 1;

static void A358425_push(size_t v)
{
    if (A358425_len + 1 >= A358425_cap) {
        size_t nc = A358425_cap * 2;
        A358425_L = (size_t *)realloc(A358425_L, nc * sizeof(size_t));
        A358425_F = (size_t *)realloc(A358425_F, (nc + 1) * sizeof(size_t));
        memset(A358425_F + A358425_cap + 1, 0, (nc - A358425_cap) * sizeof(size_t));
        A358425_cap = nc;
    }
    A358425_L[A358425_len++] = v;
    A358425_F[v] += 1;
    if (v > A358425_maxv) A358425_maxv = v;
    if (v == 7) {
        if (A358425_pn == A358425_pcap) {
            A358425_pcap *= 2;
            A358425_P = (size_t *)realloc(A358425_P, A358425_pcap * sizeof(size_t));
        }
        A358425_P[A358425_pn++] = A358425_len;
    }
}

seq_t a(idx_t n)
{
    if (A358425_L == NULL) {
        A358425_cap = 1024;
        A358425_L = (size_t *)malloc(A358425_cap * sizeof(size_t));
        A358425_F = (size_t *)calloc(A358425_cap + 1, sizeof(size_t));
        A358425_pcap = 256;
        A358425_P = (size_t *)malloc(A358425_pcap * sizeof(size_t));
        A358425_kvcap = 1024;
        A358425_kv = (size_t *)malloc(2 * A358425_kvcap * sizeof(size_t));
        A358425_push(1);
    }
    while (A358425_pn < (size_t)n) {
        size_t m = 0;
        for (size_t v = 1; v <= A358425_maxv; ++v) {
            if (A358425_F[v] != 0) {
                if (m == A358425_kvcap) {
                    A358425_kvcap *= 2;
                    A358425_kv = (size_t *)realloc(A358425_kv, 2 * A358425_kvcap * sizeof(size_t));
                }
                A358425_kv[m++] = A358425_F[v];
            }
        }
        for (size_t i = 0; i < m; ++i) A358425_push(A358425_kv[i]);
    }
    return (seq_t)A358425_P[(size_t)n - 1];
}
