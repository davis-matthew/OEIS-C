/*
 * A333268  Indices of unique values in A329152.
 * https://oeis.org/A333268
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k such that A329152(k) occurs exactly once in
 *            A329152, where A329152(k) = (phi(k) - C(k))/2 and C(k) = number
 *            of solutions of x^2 == 1 (mod k); computed by a sieve up to a
 *            limit that is doubled until enough terms are certified
 * Range:     u8 n <= 2, u16 n <= 35, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Validated: partial, pass, 35 terms through n = 35 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static unsigned long long *g_terms = 0;
static size_t g_cnt = 0;
static size_t g_L = 0;

static void A333268_build(size_t L)
{
    uint32_t *phi = (uint32_t *)malloc((L + 1) * sizeof(uint32_t));
    unsigned char *oc = (unsigned char *)calloc(L + 1, 1);
    uint32_t *val = (uint32_t *)malloc((L + 1) * sizeof(uint32_t));
    uint32_t *freq = (uint32_t *)calloc(L / 2 + 2, sizeof(uint32_t));
    size_t i, j;
    for (i = 0; i <= L; i++) phi[i] = (uint32_t)i;
    for (i = 2; i <= L; i++) {
        if (phi[i] == (uint32_t)i) {
            for (j = i; j <= L; j += i) {
                phi[j] -= phi[j] / (uint32_t)i;
                if (i != 2) oc[j]++;
            }
        }
    }
    val[0] = 0;
    for (i = 1; i <= L; i++) {
        unsigned e2 = 0;
        size_t t = i;
        uint32_t C;
        while (!(t & 1)) { e2++; t >>= 1; }
        C = (uint32_t)1 << oc[i];
        if (e2 == 2) C <<= 1;
        else if (e2 >= 3) C <<= 2;
        val[i] = (phi[i] - C) / 2;
        freq[val[i]]++;
    }
    free(g_terms);
    g_terms = (unsigned long long *)malloc((L / 16 + 2) * sizeof(unsigned long long));
    g_cnt = 0;
    for (i = 1; i <= L / 16; i++) {
        if (freq[val[i]] == 1)
            g_terms[g_cnt++] = (unsigned long long)i;
    }
    g_L = L;
    free(phi); free(oc); free(val); free(freq);
}

seq_t a(idx_t n)
{
    if (g_L == 0) A333268_build((size_t)1 << 16);
    while ((size_t)n > g_cnt)
        A333268_build(g_L * 2);
    return (seq_t)g_terms[(size_t)n - 1];
}
