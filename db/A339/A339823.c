/*
 * A339823  a(n) = A056239(n) - A000523(n).
 * https://oeis.org/A339823
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A056239(n) - A000523(n); A056239(n) = Sum_k k*c_k for n =
 *            Prod prime(k)^c_k, A000523(n) = floor(log_2(n)). Uses a sieve
 *            for prime-counting and trial division
 * Range:     u8 n <= 1698, u16 n <= 65536, u32 n <= 65536, u64 n <= 65536,
 *            u128 n <= 65536, big any n
 * Validated: standard, pass, 121 terms through n = 9724 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint32_t *pi_tab = NULL;
static uint64_t pi_lim = 0;

static void ensure_pi(uint64_t limit)
{
    if (pi_tab && limit <= pi_lim) return;
    if (limit < 16) limit = 16;
    unsigned char *comp = (unsigned char *)calloc((size_t)limit + 1, 1);
    uint32_t *t = (uint32_t *)calloc((size_t)limit + 1, sizeof(uint32_t));
    if (!comp || !t) { free(comp); free(t); return; }
    for (uint64_t i = 2; i * i <= limit; ++i)
        if (!comp[i])
            for (uint64_t j = i * i; j <= limit; j += i) comp[j] = 1;
    uint32_t c = 0;
    for (uint64_t i = 2; i <= limit; ++i) {
        if (!comp[i]) ++c;
        t[i] = c;
    }
    free(comp);
    free(pi_tab);
    pi_tab = t;
    pi_lim = limit;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    ensure_pi((uint64_t)n);
    if (!pi_tab) return 0;

    uint64_t sum = 0;
    uint64_t temp = (uint64_t)n;
    for (uint64_t p = 2; p * p <= temp; ++p) {
        if (temp % p != 0) continue;
        uint64_t e = 0;
        while (temp % p == 0) { temp /= p; ++e; }
        sum += (uint64_t)pi_tab[p] * e;
    }
    if (temp > 1) sum += pi_tab[temp];

    uint64_t lg = 0;
    uint64_t m = (uint64_t)n;
    while (m > 1) { m >>= 1; ++lg; }

    return (seq_t)(sum - lg);
}
