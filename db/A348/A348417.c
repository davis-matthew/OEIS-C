/*
 * A348417  Number of coprime squares modulo A081754(n): a(n) =
 *          A046073(A081754(n)).
 * https://oeis.org/A348417
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A046073(A081754(n)); A081754(n) = n-th k not divisible by
 *            16 with no prime factor == 1 (mod 4); A046073 multiplicative:
 *            p^e -> (p-1)p^(e-1)/2 for odd p, 2^e -> 2^max(e-3,0). Found by
 *            incremental search
 * Range:     u8 n <= 220, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 91 terms through n = 9991 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static int A348417_valid(uint64_t k)
{
    if (k % 16 == 0) return 0;
    while ((k & 1) == 0) k >>= 1;
    for (uint64_t d = 3; d * d <= k; d += 2) {
        if (k % d == 0) {
            if ((d & 3) == 1) return 0;
            while (k % d == 0) k /= d;
        }
    }
    if (k > 1 && (k & 3) == 1) return 0;
    return 1;
}

static seq_t A348417_a046073(uint64_t k)
{
    seq_t r = (seq_t)1;
    while ((k & 1) == 0) k >>= 1; /* 2^e with e<=3 contributes 1 */
    for (uint64_t d = 3; d * d <= k; d += 2) {
        if (k % d == 0) {
            seq_t v = (seq_t)((d - 1) / 2);
            k /= d;
            while (k % d == 0) { k /= d; v *= (seq_t)d; }
            r *= v;
        }
    }
    if (k > 1) r *= (seq_t)((k - 1) / 2);
    return r;
}

seq_t a(idx_t n)
{
    static uint64_t *terms = NULL;
    static uint64_t cnt = 0, cap = 0, next = 1;
    while (cnt < (uint64_t)n) {
        if (A348417_valid(next)) {
            if (cnt == cap) {
                cap = cap ? cap * 2 : 1024;
                terms = (uint64_t *)realloc(terms, cap * sizeof(uint64_t));
                if (!terms) { cnt = cap = 0; next = 1; return 0; }
            }
            terms[cnt++] = next;
        }
        next++;
    }
    return A348417_a046073(terms[n - 1]);
}
