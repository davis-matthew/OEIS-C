/*
 * A252370  a(n) = A243055(A251726(n)).
 * https://oeis.org/A252370
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A243055(A251726(n)); A251726 = integers m>1 with gpf(m) <
 *            lpf(m)^2 (found by a scan with trial division), A243055(m) =
 *            pi(gpf(m)) - pi(lpf(m))
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 136 terms through n = 9630 (2026-11-07)
 * Helpers:   prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/prime_pi.h"

static void A252370_factor(uint64_t m, uint64_t *lo, uint64_t *hi)
{
    uint64_t l = 0, g = 1;
    for (uint64_t p = 2; p <= m / p; ++p) {
        if (m % p == 0) {
            if (!l) l = p;
            g = p;
            while (m % p == 0) m /= p;
        }
    }
    if (m > 1) {
        if (!l) l = m;
        g = m;
    }
    *lo = l;
    *hi = g;
}

static uint64_t *A252370_terms = NULL;
static size_t A252370_len = 0, A252370_cap = 0;
static uint64_t A252370_next = 2;

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    while (A252370_len < (size_t)n) {
        uint64_t l, g;
        A252370_factor(A252370_next, &l, &g);
        if (l <= UINT64_MAX / l && g >= l * l) {
            /* does not qualify */
        } else {
            if (A252370_len >= A252370_cap) {
                size_t nc = A252370_cap ? A252370_cap * 2 : 1024;
                uint64_t *t = (uint64_t *)realloc(A252370_terms, nc * sizeof(uint64_t));
                if (!t) exit(1);
                A252370_terms = t;
                A252370_cap = nc;
            }
            A252370_terms[A252370_len++] = A252370_next;
        }
        ++A252370_next;
    }
    uint64_t m = A252370_terms[(size_t)n - 1];
    uint64_t l, g;
    A252370_factor(m, &l, &g);
    if (l == g) return (seq_t)0;
    seq_t i = prime_pi((idx_t)l);
    seq_t k = prime_pi((idx_t)g);
    return k - i;
}
