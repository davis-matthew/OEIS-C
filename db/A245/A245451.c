/*
 * A245451  Self-inverse permutation of nonnegative integers,
 *          A075158-conjugate of gray code: a(n) = 1 +
 *          A075157(A003188(A075158(n-1))).
 * https://oeis.org/A245451
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + A075157(A003188(A075158(n-1))); computed directly on
 *            bit arrays (the +1 and -1 cancel)
 * Range:     u8 n <= 18, u16 n <= 52, u32 n <= 130, u64 n <= 310,
 *            u128 n <= 718, big any n
 * Validated: standard, pass, 80 terms through n = 978 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;

    /* factor n (= (n-1)+1) */
    idx_t t = n;
    idx_t fp[70];
    int fe[70];
    int fc = 0;
    for (idx_t d = 2; d <= t / d; d++) {
        if (t % d == 0) {
            int e = 0;
            while (t % d == 0) { t /= d; e++; }
            fp[fc] = d; fe[fc] = e; fc++;
        }
    }
    if (t > 1) { fp[fc] = t; fe[fc] = 1; fc++; }
    idx_t P = fp[fc - 1];

    /* sieve primes up to P, collect exponents */
    unsigned char *comp = (unsigned char *)calloc((size_t)P + 1, 1);
    unsigned char *ex = (unsigned char *)calloc((size_t)P + 1, 1);
    size_t cnt = 0;
    for (idx_t p = 2; p <= P; p++) {
        if (!comp[p]) {
            for (idx_t j = p; j <= P / p; j++) comp[j * p] = 1;
            int e = 0;
            for (int q = 0; q < fc; q++) if (fp[q] == p) e = fe[q];
            ex[cnt++] = (unsigned char)e;
        }
    }
    free(comp);
    size_t L = cnt - 1;

    /* bits of A075158(n-1), MSB first */
    size_t total = ex[L];
    for (size_t i = 0; i < L; i++) total += (size_t)ex[i] + 1;
    unsigned char *b = (unsigned char *)malloc(total + 1);
    size_t pos = 0;
    for (int i = 0; i < ex[L]; i++) b[pos++] = 1;
    int ones = 0;
    for (size_t i = L; i-- > 0; ) {
        int len = (int)ex[i] + 1;
        for (int j = 0; j < len; j++) b[pos++] = (unsigned char)ones;
        ones ^= 1;
    }
    free(ex);

    /* Gray code: g[i] = b[i] ^ b[i-1], in place from the end */
    for (size_t i = total; i-- > 1; ) b[i] ^= b[i - 1];

    /* count runs */
    size_t runs = 0;
    for (size_t i = 0; i < total; i++)
        if (i == 0 || b[i] != b[i - 1]) runs++;

    /* first `runs` primes */
    unsigned int *pr = (unsigned int *)malloc((runs + 1) * sizeof(unsigned int));
    size_t pc = 0;
    for (unsigned int c = 2; pc < runs; c++) {
        bool ok = true;
        for (size_t q = 0; q < pc && (unsigned long long)pr[q] * pr[q] <= c; q++)
            if (c % pr[q] == 0) { ok = false; break; }
        if (ok) pr[pc++] = c;
    }

    seq_t prod = (seq_t)1;
    long long ps = (long long)total - 1;
    size_t ri = 0;
    while (ps >= 0) {
        unsigned char bit = b[ps];
        long long len = 0;
        while (ps >= 0 && b[ps] == bit) { len++; ps--; }
        long long e = (ps < 0 && bit == 1) ? len : len - 1;
        for (long long k = 0; k < e; k++) prod *= (seq_t)pr[ri];
        ri++;
    }

    free(pr);
    free(b);
    return prod;
}
