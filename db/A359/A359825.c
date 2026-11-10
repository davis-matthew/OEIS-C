/*
 * A359825  Positions of odd terms in A359823, where A359823 is the Dirichlet
 *          inverse of A359820.
 * https://oeis.org/A359825
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = position of the n-th odd term of A359823 (Dirichlet
 *            inverse of A359820), found by a parity sieve mod 2: g(1)=1, g(m)
 *            = sum_{d|m, d<m} A359820(m/d)*g(d) mod 2
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Validated: partial, pass, 73 terms through n = 73 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static idx_t *pos_list = NULL;
static idx_t pos_count = 0;

static int parity_f(idx_t m)
{
    idx_t t = m;
    int par = 0;
    for (idx_t p = 2; p * p <= t; p++) {
        if (t % p == 0) {
            int e = 0;
            while (t % p == 0) { t /= p; e++; }
            if ((e & 1) && ((m / p) & 1)) par ^= 1;
        }
    }
    if (t > 1 && ((m / t) & 1)) par ^= 1;
    return (int)((m & 1) ^ (idx_t)par);
}

static void build(idx_t L)
{
    unsigned char *f = (unsigned char *)malloc((size_t)L + 1);
    unsigned char *g = (unsigned char *)malloc((size_t)L + 1);
    idx_t i, k, d;
    for (i = 0; i <= L; i++) { f[i] = 0; g[i] = 0; }
    for (i = 1; i <= L; i++) f[i] = (unsigned char)parity_f(i);
    g[1] = 1;
    free(pos_list);
    pos_list = (idx_t *)malloc(((size_t)L + 1) * sizeof(idx_t));
    pos_count = 0;
    for (d = 1; d <= L; d++) {
        if (g[d]) {
            pos_list[pos_count++] = d;
            for (k = 2; d * k <= L; k++)
                if (f[k]) g[d * k] ^= 1;
        }
    }
    free(f);
    free(g);
}

seq_t a(idx_t n)
{
    static idx_t L = 0;
    if (n == 0) return (seq_t)0;
    while (pos_count < n) {
        L = (L == 0) ? 512 : L * 2;
        build(L);
    }
    return (seq_t)pos_list[n - 1];
}
