/*
 * A392317  Partial sums of A393434.
 * https://oeis.org/A392317
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=0..n} A393434(k); A393434 built by greedy search
 *            (a(0)=0, least unused |value| at distance k!, tie -> positive)
 * Range:     i8 n <= 2, i16 n <= 2, i32 n <= 9000, i64 n <= 9000,
 *            i128 n <= 9000, big any n
 * Validated: standard, pass, 77 terms through n = 5982 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/mem.h"

static seq_t *A392317_terms = NULL;   /* prefix sums */
static size_t A392317_len = 0, A392317_cap = 0;
static seq_t A392317_prev = 0;
static unsigned char *A392317_used = NULL;
static long long A392317_B = 0;

static void A392317_grow(long long need)
{
    long long nb = A392317_B ? A392317_B : 65536;
    while (nb < need) nb *= 2;
    if (nb == A392317_B) return;
    unsigned char *nu = (unsigned char *)calloc((size_t)(2 * nb + 1), 1);
    if (A392317_used) {
        for (long long v = -A392317_B; v <= A392317_B; ++v)
            nu[v + nb] = A392317_used[v + A392317_B];
        free(A392317_used);
    }
    A392317_used = nu;
    A392317_B = nb;
}

static bool A392317_isused(seq_t x)
{
    seq_t ax = x < 0 ? -x : x;
    if (ax > (seq_t)A392317_B) return false;
    return A392317_used[(long long)x + A392317_B] != 0;
}

static void A392317_init(void)
{
    if (A392317_len) return;
    A392317_grow(65536);
    A392317_used[A392317_B] = 1;
    A392317_cap = 16;
    A392317_terms = mem_new(seq_t, A392317_cap);
    A392317_terms[0] = 0;
    A392317_len = 1;
    A392317_prev = 0;
}

seq_t a(idx_t n)
{
    A392317_init();
    size_t target = (size_t)n;
    while (A392317_len <= target) {
        seq_t prev = A392317_prev;
        seq_t aprev = prev < 0 ? -prev : prev;
        bool found = false;
        seq_t best = 0, best_abs = 0;
        seq_t f = 1;
        for (seq_t k = 1;; ++k) {
            f = f * k;
            for (int s = 0; s < 2; ++s) {
                seq_t cand = s ? prev - f : prev + f;
                if (A392317_isused(cand)) continue;
                seq_t ca = cand < 0 ? -cand : cand;
                if (!found || ca < best_abs || (ca == best_abs && cand > 0 && best < 0)) {
                    best = cand; best_abs = ca; found = true;
                }
            }
            if (found && f > aprev + best_abs) break;
        }
        A392317_grow((long long)best_abs);
        A392317_used[(long long)best + A392317_B] = 1;
        A392317_prev = best;
        if (A392317_len >= A392317_cap) {
            A392317_cap *= 2;
            A392317_terms = mem_resize(seq_t, A392317_terms, A392317_cap);
        }
        A392317_terms[A392317_len] = A392317_terms[A392317_len - 1] + best;
        ++A392317_len;
    }
    return A392317_terms[target];
}
