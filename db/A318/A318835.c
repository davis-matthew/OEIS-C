/*
 * A318835  Restricted growth sequence transform of A318834, product_{d|n,
 *          d<n} A019565(A000010(d)).
 * https://oeis.org/A318835
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = RGS transform of A318834(n) = Product_{d|n, d<n}
 *            A019565(A000010(d)); computed by a sieve over exponent vectors
 *            of the primes in each product, labelling distinct vectors in
 *            order of first appearance
 * Range:     u8 n <= 343, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 113 terms through n = 9322 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define A318835_W 24

static uint32_t *A318835_labels = 0;
static idx_t A318835_cap = 0;

static void A318835_build(idx_t N)
{
    size_t W = A318835_W;
    size_t sz = (size_t)N + 1;
    uint32_t *phi = (uint32_t *)malloc(sz * sizeof(uint32_t));
    uint32_t *vec = (uint32_t *)calloc(sz * W, sizeof(uint32_t));
    size_t hs = 1;
    size_t i, m, d;
    uint32_t *tab;
    uint32_t *lab;
    uint32_t next = 0;

    while (hs < 2 * sz) hs <<= 1;
    tab = (uint32_t *)calloc(hs, sizeof(uint32_t));
    lab = (uint32_t *)calloc(sz, sizeof(uint32_t));

    for (i = 0; i < sz; i++) phi[i] = (uint32_t)i;
    for (i = 2; i < sz; i++) {
        if (phi[i] == (uint32_t)i) {
            for (m = i; m < sz; m += i) phi[m] -= phi[m] / (uint32_t)i;
        }
    }
    for (d = 1; d <= (size_t)N / 2; d++) {
        uint32_t ph = phi[d];
        for (m = 2 * d; m < sz; m += d) {
            uint32_t *v = vec + m * W;
            uint32_t b = ph;
            size_t k = 0;
            while (b) {
                if (b & 1u) v[k]++;
                b >>= 1;
                k++;
            }
        }
    }
    for (m = 1; m < sz; m++) {
        uint32_t *v = vec + m * W;
        uint64_t h = 1469598103934665603ULL;
        size_t k, pos;
        for (k = 0; k < W; k++) {
            h ^= v[k];
            h *= 1099511628211ULL;
        }
        h ^= h >> 29;
        pos = (size_t)(h & (hs - 1));
        for (;;) {
            uint32_t rep = tab[pos];
            if (rep == 0) {
                tab[pos] = (uint32_t)m;
                lab[m] = ++next;
                break;
            }
            if (memcmp(vec + (size_t)rep * W, v, W * sizeof(uint32_t)) == 0) {
                lab[m] = lab[rep];
                break;
            }
            pos = (pos + 1) & (hs - 1);
        }
    }
    free(phi);
    free(vec);
    free(tab);
    if (A318835_labels) free(A318835_labels);
    A318835_labels = lab;
    A318835_cap = N;
}

seq_t a(idx_t n)
{
    if (n > A318835_cap) {
        idx_t N = n;
        if (N < 2 * A318835_cap) N = 2 * A318835_cap;
        if (N < 1024) N = 1024;
        A318835_build(N);
    }
    return (seq_t)A318835_labels[n];
}
