/*
 * A328530  G.f. = Phi^5, where Phi = g.f. for A028930.
 * https://oeis.org/A328530
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [q^n] Phi(q)^5, Phi = theta series of 2x^2+xy+3y^2
 *            (A028930); computed as a(n) = Sum_{k=0..n} Phi^2[k] * Phi^3[n-k]
 * Range:     u8 n <= 6, u16 n <= 25, u32 n <= 401, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Validated: standard, pass, 55 terms through n = 4854 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    long long m = (long long)n;
    long long L = 0, i, j;
    unsigned long long *P, *P2, *P3, res = 0;
    while ((L + 1) * (L + 1) <= m) L++;
    L++;
    P  = (unsigned long long *)calloc((size_t)(m + 1), sizeof(unsigned long long));
    P2 = (unsigned long long *)calloc((size_t)(m + 1), sizeof(unsigned long long));
    P3 = (unsigned long long *)calloc((size_t)(m + 1), sizeof(unsigned long long));
    for (long long y = -L; y <= L; y++)
        for (long long x = -L; x <= L; x++) {
            long long v = 2 * x * x + x * y + 3 * y * y;
            if (v <= m) P[v]++;
        }
    /* P2 = P*P, P3 = P2*P using sparsity of P */
    for (j = 0; j <= m; j++) {
        if (!P[j]) continue;
        for (i = 0; i + j <= m; i++)
            if (P[i]) P2[i + j] += P[i] * P[j];
    }
    for (j = 0; j <= m; j++) {
        if (!P[j]) continue;
        for (i = 0; i + j <= m; i++)
            P3[i + j] += P2[i] * P[j];
    }
    for (i = 0; i <= m; i++)
        res += P2[i] * P3[m - i];
    free(P);
    free(P2);
    free(P3);
    return (seq_t)res;
}
