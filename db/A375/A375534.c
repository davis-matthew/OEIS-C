/*
 * A375534  a(n) = denominator of Sum_{i=1..n} 1/A038603(i).
 * https://oeis.org/A375534
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = Product_{i=1..n} A038603(i), since the sum of
 *            1/A038603(i) stays in lowest terms with denominator equal to the
 *            product of the distinct primes (a(0)=1)
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 22,
 *            big any n
 * Validated: standard, pass, 35 terms through n = 232 (2026-11-10)
 * Helpers:   is_prime, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/mem.h"

static bool A375534_has_one(idx_t x) {
    while (x > 0) {
        if ((x % 10) == 1) return true;
        x /= 10;
    }
    return false;
}

static seq_t *A375534_prod = NULL;
static size_t A375534_len = 0;
static size_t A375534_cap = 0;
static idx_t A375534_cand = 2;

seq_t a(idx_t n) {
    size_t need = (size_t)n + 1;
    if (A375534_cap < need) {
        size_t nc = A375534_cap ? A375534_cap * 2 : 64;
        if (nc < need) nc = need;
        seq_t *p = mem_resize(seq_t, A375534_prod, nc);
        if (!p) abort();
        A375534_prod = p;
        A375534_cap = nc;
    }
    if (A375534_len == 0) {
        A375534_prod[0] = (seq_t)1;
        A375534_len = 1;
    }
    while (A375534_len < need) {
        for (;;) {
            if (!A375534_has_one(A375534_cand) && is_prime(A375534_cand)) {
                A375534_prod[A375534_len] = A375534_prod[A375534_len - 1] * (seq_t)A375534_cand;
                A375534_len++;
                A375534_cand++;
                break;
            }
            A375534_cand++;
        }
    }
    return A375534_prod[n];
}
