/*
 * A286448  Restricted growth sequence computed for A252748 (= A003961(n) -
 *          2*n).
 * https://oeis.org/A286448
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = restricted growth sequence of A252748, where A252748(n) =
 *            A003961(n) - 2*n; a(n) = number of distinct values among
 *            A252748(1..f), f = first index with A252748(f) = A252748(n)
 * Range:     u8 n <= 359, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 112 terms through n = 8303 (2026-11-08)
 * Helpers:   is_prime_u64, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_u64.h"
#include "../utils/next_prime.h"

typedef __int128 rgs_val_t;

static rgs_val_t A252748_val(uint64_t n) {
    rgs_val_t result = 1;
    uint64_t m = n;
    for (uint64_t p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            uint64_t q = next_prime(p);
            while (m % p == 0) {
                m /= p;
                result *= (rgs_val_t)q;
            }
        }
    }
    if (m > 1) result *= (rgs_val_t)next_prime(m);
    return result - (rgs_val_t)2 * (rgs_val_t)n;
}

static int rgs_cmp(const void *x, const void *y) {
    rgs_val_t a = *(const rgs_val_t *)x, b = *(const rgs_val_t *)y;
    return (a > b) - (a < b);
}

seq_t a(idx_t n) {
    size_t N = (size_t)n;
    rgs_val_t *v = (rgs_val_t *)malloc((N + 1) * sizeof(rgs_val_t));
    rgs_val_t *s = (rgs_val_t *)malloc((N + 1) * sizeof(rgs_val_t));
    for (size_t i = 1; i <= N; i++) v[i] = A252748_val(i);
    size_t f = 1;
    while (v[f] != v[N]) f++;
    for (size_t i = 1; i <= f; i++) s[i - 1] = v[i];
    qsort(s, f, sizeof(rgs_val_t), rgs_cmp);
    size_t cnt = 1;
    for (size_t i = 1; i < f; i++) if (s[i] != s[i - 1]) cnt++;
    free(v);
    free(s);
    return (seq_t)cnt;
}
