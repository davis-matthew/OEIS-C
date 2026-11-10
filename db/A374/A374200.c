/*
 * A374200  a(n) is the minimum value of A347381 that it obtains among the
 *          unitary divisors of n larger than 1, where A347381 is the distance
 *          from n to the nearest common ancestor of n and sigma(n) in the
 *          Doudna-tree (A005940). By convention a(1) = 0.
 * https://oeis.org/A374200
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 0, and for n > 1, a(n) = Min_{d|n, d>1, gcd(d,n/d)=1}
 *            A347381(d); A347381 computed via binary heap paths of the Doudna
 *            tree (A005940), distance = depth(d) - common-prefix length with
 *            sigma(d)
 * Range:     u8 n <= 1636, u16 n <= 100000, u32 n <= 100000, u64 n <= 100000,
 *            u128 n <= 100000, big any n
 * Validated: standard, pass, 117 terms through n = 9541 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* Build MSB-first bit string (heap path) of the Doudna-tree node holding value v.
   Bit at position pos (from LSB) is 1 for each prime factor (ascending index k, j-th one):
   pos = k-1 + (j-1). Returns length; out[0] is the top bit. */
static size_t A347381_path(uint64_t v, const uint32_t *pidx, unsigned char *out, size_t cap) {
    size_t top = 0;
    uint64_t j = 0;
    uint64_t p;
    memset(out, 0, cap);
    /* store by position first (LSB-first), reverse later */
    for (p = 2; p * p <= v; p++) {
        while (v % p == 0) {
            size_t pos = (size_t)pidx[p] - 1 + (size_t)j;
            out[pos] = 1;
            if (pos + 1 > top) top = pos + 1;
            j++;
            v /= p;
        }
    }
    if (v > 1) {
        size_t pos = (size_t)pidx[v] - 1 + (size_t)j;
        out[pos] = 1;
        if (pos + 1 > top) top = pos + 1;
    }
    /* reverse to MSB-first */
    {
        size_t a = 0, b = top ? top - 1 : 0;
        while (a < b) {
            unsigned char t = out[a]; out[a] = out[b]; out[b] = t;
            a++; b--;
        }
    }
    return top;
}

seq_t a(idx_t n) {
    if (n <= 1) return (seq_t)0;
    uint64_t N = (uint64_t)n;
    uint64_t pr[24], pw[24];
    uint64_t sg[24];
    int cnt = 0;
    uint64_t m = N, p;
    for (p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            uint64_t q = 1, s = 1, t = 1;
            while (m % p == 0) { m /= p; t *= p; s += t; q = t; }
            pr[cnt] = p; pw[cnt] = q; sg[cnt] = s; cnt++;
        }
    }
    if (m > 1) { pr[cnt] = m; pw[cnt] = m; sg[cnt] = m + 1; cnt++; }
    uint64_t sigN = 1;
    for (int i = 0; i < cnt; i++) sigN *= sg[i];
    uint64_t limit = sigN > N ? sigN : N;
    uint32_t *pidx = (uint32_t *)calloc((size_t)limit + 2, sizeof(uint32_t));
    unsigned char *comp = (unsigned char *)calloc((size_t)limit + 2, 1);
    uint32_t c = 0;
    for (uint64_t i = 2; i <= limit; i++) {
        if (!comp[i]) {
            c++;
            pidx[i] = c;
            for (uint64_t k = i * i; k <= limit; k += i) comp[k] = 1;
        }
    }
    size_t cap = (size_t)c + 100;
    unsigned char *b1 = (unsigned char *)malloc(cap);
    unsigned char *b2 = (unsigned char *)malloc(cap);
    size_t best = (size_t)-1;
    for (uint32_t mask = 1; mask < (1u << cnt); mask++) {
        uint64_t d = 1, s = 1;
        for (int i = 0; i < cnt; i++) if (mask & (1u << i)) { d *= pw[i]; s *= sg[i]; }
        size_t L1 = A347381_path(d, pidx, b1, cap);
        size_t L2 = A347381_path(s, pidx, b2, cap);
        size_t cp = 0;
        while (cp < L1 && cp < L2 && b1[cp] == b2[cp]) cp++;
        size_t dist = L1 - cp;
        if (dist < best) best = dist;
    }
    free(b1); free(b2); free(pidx); free(comp);
    return (seq_t)best;
}
