/*
 * A396932  Decimal expansion of the sum of the reciprocals of A279663.
 * https://oeis.org/A396932
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = n-th decimal digit of Sum_{k>=0} 1/A279663(k), where
 *            A279663(k) = Product_{j=1..k} j*(j+1)*(5j-2)/6 (equals
 *            hypergeom([], [3/5, 2], 6/5)); computed by fixed-point base-1e9
 *            arithmetic
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 127 terms through n = 8195 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    size_t digits = (size_t)n + 12;
    size_t F = digits / 9 + 2;      /* fractional limbs */
    size_t L = F + 1;               /* limb 0 = integer part */
    const uint64_t B = 1000000000ULL;
    uint32_t *S = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *T = (uint32_t *)calloc(L, sizeof(uint32_t));
    if (!S || !T) { free(S); free(T); return 0; }
    S[0] = 1;
    T[0] = 1;
    for (uint64_t k = 1;; ++k) {
        uint64_t divs[3];
        divs[0] = k; divs[1] = k + 1; divs[2] = 5 * k - 2;
        /* multiply by 6 */
        uint64_t carry = 0;
        for (size_t i = L; i-- > 0;) {
            uint64_t v = (uint64_t)T[i] * 6 + carry;
            T[i] = (uint32_t)(v % B);
            carry = v / B;
        }
        for (int j = 0; j < 3; ++j) {
            uint64_t rem = 0;
            for (size_t i = 0; i < L; ++i) {
                uint64_t v = rem * B + T[i];
                T[i] = (uint32_t)(v / divs[j]);
                rem = v % divs[j];
            }
        }
        int zero = 1;
        for (size_t i = 0; i < L; ++i) if (T[i]) { zero = 0; break; }
        if (zero) break;
        carry = 0;
        for (size_t i = L; i-- > 0;) {
            uint64_t v = (uint64_t)S[i] + T[i] + carry;
            S[i] = (uint32_t)(v % B);
            carry = v / B;
        }
    }
    int res;
    if (n == 1) {
        res = (int)(S[0] % 10);
    } else {
        size_t j = (size_t)n - 1;           /* fractional digit index, 1-based */
        size_t limb = 1 + (j - 1) / 9;
        int pos = (int)((j - 1) % 9);
        uint32_t d = S[limb];
        for (int p = 0; p < 8 - pos; ++p) d /= 10;
        res = (int)(d % 10);
    }
    free(S);
    free(T);
    return (seq_t)res;
}
