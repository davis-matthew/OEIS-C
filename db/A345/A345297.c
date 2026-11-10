/*
 * A345297  a(n) is the least k >= 0 such that A331835(k) = n.
 * https://oeis.org/A345297
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = least k >= 0 with A331835(k) = n, i.e. the smallest
 *            binary number whose set bits j (values 1,2,3,5,7,11,... = 1,
 *            prime(j)) sum to n; computed by a subset-sum reachability table
 *            and greedy lowest-top-bit reconstruction
 * Range:     u8 n <= 59, u16 n <= 329, u32 n <= 1721, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Validated: standard, pass, 76 terms through n = 1861 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    size_t N = (size_t)n;

    /* values v[0]=1, v[j]=prime(j) for primes <= N */
    unsigned char *comp = (unsigned char *)calloc(N + 2, 1);
    size_t *v = (size_t *)malloc((N + 2) * sizeof(size_t));
    if (!comp || !v) { free(comp); free(v); return (seq_t)0; }
    size_t J = 0;
    v[J++] = 1;
    for (size_t i = 2; i <= N; i++) {
        if (!comp[i]) {
            v[J++] = i;
            for (size_t j = i * i; j <= N && i * i <= N; j += i) comp[j] = 1;
        }
    }
    free(comp);

    size_t W = N + 1;
    unsigned char *reach = (unsigned char *)calloc((J + 1) * W, 1);
    if (!reach) { free(v); return (seq_t)0; }
    reach[0] = 1;
    for (size_t j = 0; j < J; j++) {
        unsigned char *cur = reach + j * W;
        unsigned char *nxt = reach + (j + 1) * W;
        size_t vj = v[j];
        for (size_t m = 0; m <= N; m++) {
            nxt[m] = cur[m] || (m >= vj && cur[m - vj]);
        }
    }

    seq_t result = 0;
    size_t r = N;
    size_t limit = J;
    while (r > 0) {
        size_t chosen = limit;
        for (size_t t = 0; t < limit; t++) {
            if (v[t] <= r && reach[t * W + (r - v[t])]) { chosen = t; break; }
        }
        if (chosen == limit) break; /* should not happen */
        result += ((seq_t)1) << chosen;
        r -= v[chosen];
        limit = chosen;
    }

    free(reach);
    free(v);
    return result;
}
