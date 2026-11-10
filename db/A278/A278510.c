/*
 * A278510  a(n) = A258851(n) - A056239(n).
 * https://oeis.org/A278510
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A258851(n) - A056239(n) = Sum_{p^e || n} e*pi(p)*(n/p -
 *            1), pi = A000720
 * Range:     u8 n <= 95, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 110 terms through n = 9519 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

static idx_t a278510_lim = 0;
static uint32_t *a278510_pi = NULL;

static void a278510_ensure(idx_t limit)
{
    if (limit <= a278510_lim) return;
    idx_t L = limit < 1024 ? 1024 : limit;
    unsigned char *comp = (unsigned char *)calloc((size_t)L + 1, 1);
    uint32_t *pi = (uint32_t *)calloc((size_t)L + 1, sizeof(uint32_t));
    if (!comp || !pi) { free(comp); free(pi); return; }
    for (idx_t i = 2; i * i <= L; ++i)
        if (!comp[i])
            for (idx_t j = i * i; j <= L; j += i) comp[j] = 1;
    uint32_t c = 0;
    for (idx_t i = 2; i <= L; ++i) {
        if (!comp[i]) ++c;
        pi[i] = c;
    }
    free(comp);
    free(a278510_pi);
    a278510_pi = pi;
    a278510_lim = L;
}

seq_t a(idx_t n)
{
    if (n <= 1) return 0;
    a278510_ensure(n);
    seq_t result = 0;
    idx_t m = n;
    for (idx_t p = 2; p * p <= m; ++p) {
        if (m % p != 0) continue;
        idx_t e = 0;
        while (m % p == 0) { m /= p; ++e; }
        result += (seq_t)e * (seq_t)a278510_pi[p] * ((seq_t)(n / p) - 1);
    }
    if (m > 1) {
        result += (seq_t)a278510_pi[m] * ((seq_t)(n / m) - 1);
    }
    return result;
}
