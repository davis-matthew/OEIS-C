/*
 * A344995  Numbers k such that A051709(k)/A173557(k) is a positive natural
 *          number and a divisor of k.
 * https://oeis.org/A344995
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search over k = 1,2,3,...: k is a term iff q =
 *            (sigma(k)+phi(k)-2k)/A173557(k) is a positive integer dividing
 *            k; a sieve of smallest prime factors is grown by repeated
 *            enlargement
 * Range:     u8 n <= 5, u16 n <= 28, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Validated: partial, pass, 46 terms through n = 46 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint64_t *g_terms = 0;
static size_t g_cnt = 0, g_cap = 0;
static uint64_t g_scanned = 0;
static uint64_t g_limit = 0;

static bool A344995_test(uint64_t k, const uint32_t *spf)
{
    uint64_t m = k, sigma = 1, phi = 1, rp = 1;
    while (m > 1) {
        uint64_t p = spf[m];
        uint64_t pk = 1, term = 1;
        uint64_t ph = 1;
        while (m % p == 0) {
            m /= p;
            if (pk == 1) { ph = p - 1; }
            else ph *= p;
            pk *= p;
            term += pk;
        }
        sigma *= term;
        phi *= ph;
        rp *= (p - 1);
    }
    uint64_t pos = sigma + phi;
    if (pos <= 2 * k) return false;
    uint64_t s = pos - 2 * k;
    if (s % rp != 0) return false;
    uint64_t q = s / rp;
    return q > 0 && (k % q == 0);
}

static void A344995_extend(void)
{
    uint64_t L = g_limit ? g_limit * 4 : (1u << 16);
    uint32_t *spf = (uint32_t *)malloc((size_t)(L + 1) * sizeof(uint32_t));
    if (!spf) return;
    for (uint64_t i = 0; i <= L; ++i) spf[i] = 0;
    for (uint64_t i = 2; i <= L; ++i) {
        if (spf[i] == 0) {
            for (uint64_t j = i; j <= L; j += i)
                if (spf[j] == 0) spf[j] = (uint32_t)i;
        }
    }
    for (uint64_t k = g_scanned + 1; k <= L; ++k) {
        if (k < 2) continue;
        if (A344995_test(k, spf)) {
            if (g_cnt == g_cap) {
                g_cap = g_cap ? g_cap * 2 : 64;
                g_terms = (uint64_t *)realloc(g_terms, g_cap * sizeof(uint64_t));
            }
            g_terms[g_cnt++] = k;
        }
    }
    g_scanned = L;
    g_limit = L;
    free(spf);
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    size_t before;
    while (g_cnt < (size_t)n) {
        before = g_limit;
        A344995_extend();
        if (g_limit == before) break;
    }
    if (g_cnt < (size_t)n) return (seq_t)0;
    return (seq_t)g_terms[n - 1];
}
