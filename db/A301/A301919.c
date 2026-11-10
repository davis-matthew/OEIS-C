/*
 * A301919  a(n) is the least value of k for which A301918(n) divides 3^k+3.
 * https://oeis.org/A301919
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0 if p=2, else ord_p(3)/2 + 1, where p = A301918(n) is
 *            the n-th prime dividing some 3^k+3 (ord_p(3) even; p=3 gives 1).
 *            Computed by search
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Validated: partial, pass, 62 terms through n = 62 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint64_t *A301919_res = NULL;
static size_t A301919_len = 0, A301919_cap = 0;
static uint64_t A301919_next = 2;

static uint64_t A301919_pm(uint64_t b, uint64_t e, uint64_t m)
{
    uint64_t r = 1 % m;
    b %= m;
    while (e) {
        if (e & 1) r = (r * b) % m;
        b = (b * b) % m;
        e >>= 1;
    }
    return r;
}

static bool A301919_isprime(uint64_t x)
{
    if (x < 2) return false;
    for (uint64_t d = 2; d * d <= x; d++)
        if (x % d == 0) return false;
    return true;
}

void *A301919_dummy_unused(void);
void *A301919_dummy_unused(void) { return NULL; }

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    while (A301919_len < (size_t)n) {
        uint64_t c = A301919_next++;
        if (!A301919_isprime(c)) continue;
        uint64_t k;
        bool ok = false;
        if (c == 2) { k = 0; ok = true; }
        else if (c == 3) { k = 1; ok = true; }
        else {
            uint64_t ord = c - 1, m = c - 1;
            for (uint64_t q = 2; q * q <= m; q++) {
                if (m % q == 0) {
                    while (m % q == 0) m /= q;
                    while (ord % q == 0 && A301919_pm(3, ord / q, c) == 1) ord /= q;
                }
            }
            if (m > 1) {
                while (ord % m == 0 && A301919_pm(3, ord / m, c) == 1) ord /= m;
            }
            if ((ord & 1) == 0) { ok = true; k = ord / 2 + 1; }
            else k = 0;
        }
        if (ok) {
            if (A301919_len == A301919_cap) {
                A301919_cap = A301919_cap ? A301919_cap * 2 : 16;
                A301919_res = (uint64_t *)realloc(A301919_res, A301919_cap * sizeof(uint64_t));
            }
            A301919_res[A301919_len++] = k;
        }
    }
    return (seq_t)A301919_res[(size_t)n - 1];
}
