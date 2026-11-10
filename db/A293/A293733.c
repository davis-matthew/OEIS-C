/*
 * A293733  Number of multisets of nonempty words with a total of n letters
 *          over ternary alphabet such that within each prefix of a word every
 *          letter of the alphabet is at least as frequent as the subsequent
 *          alphabet letter.
 * https://oeis.org/A293733
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   G.f.: Product_{j>=1} 1/(1-x^j)^A001006(j), expanded factor by
 *            factor as 1/(1-x^j)^m = Sum_{k>=0} binomial(m+k-1,k) x^(j*k),
 *            A001006 by M(i) = M(i-1) + Sum_{k=0}^{i-2} M(k)*M(i-2-k), M(0) =
 *            M(1) = 1
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 22, u64 n <= 43, u128 n <= 84,
 *            big any n
 * Validated: standard, pass, 46 terms through n = 913 (2026-11-08)
 * Helpers:   gcd, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/mem.h"
#include "../utils/gcd.h"

static seq_t *A293733_cache = NULL;
static size_t A293733_have = 0;

seq_t a(idx_t n) {
    size_t N = (size_t)n;
    if (A293733_cache != NULL && N < A293733_have) return A293733_cache[N];
    size_t cap = N + 1;
    if (cap < 64) cap = 64;
    if (A293733_cache != NULL) { mem_free(A293733_cache); A293733_cache = NULL; }

    seq_t *M = mem_new(seq_t, cap + 1);
    seq_t *coef = mem_new(seq_t, cap + 1);
    seq_t *r = mem_new(seq_t, cap + 1);

    M[0] = 1;
    M[1] = 1;
    for (size_t i = 2; i < cap; i++) {
        seq_t s = 0;
        for (size_t k = 0; k + 1 < i; k++) s = s + M[k] * M[i - 2 - k];
        M[i] = M[i - 1] + s;
    }

    r[0] = 1;
    for (size_t j = 1; j < cap; j++) {
        seq_t m = M[j];
        size_t kmax = (cap - 1) / j;
        coef[0] = 1;
        for (size_t k = 1; k <= kmax; k++) {
            /* coef[k] = coef[k-1] * (m+k-1) / k, exact without overflow */
            seq_t rr = coef[k - 1];
            seq_t A = m + (seq_t)(k - 1);
            seq_t kk = (seq_t)k;
            seq_t g = gcd(rr, kk);
            if (g == 0) g = 1;
            rr = rr / g;
            kk = kk / g;
            coef[k] = rr * (A / kk);
        }
        for (size_t i = cap - 1; i >= j; i--) {
            seq_t s = r[i];
            size_t idx = i;
            for (size_t k = 1; k <= i / j; k++) {
                idx -= j;
                s = s + coef[k] * r[idx];
            }
            r[i] = s;
            if (i == j) break;
        }
    }

    mem_free(M);
    mem_free(coef);
    A293733_cache = r;
    A293733_have = cap;
    return r[N];
}
