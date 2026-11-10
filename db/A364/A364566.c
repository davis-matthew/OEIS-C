/*
 * A364566  Numbers k such that A364557(k) < A000010(k), where A364557 is the
 *          Möbius transform of A005941, and A000010 is Euler totient function
 *          phi.
 * https://oeis.org/A364566
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k with A364557(k) < A000010(k), where A364557(k) =
 *            2^((# of 2's in k)-1 + Sum_{odd p^e || k} (e-1) + pi(max odd
 *            prime of k)-1) (k>1) and A000010 = phi; found by incremental
 *            search
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Validated: partial, pass, 69 terms through n = 69 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint32_t *a364566_pi = NULL;
static uint64_t a364566_cap = 0;

static void a364566_ensure(uint64_t lim)
{
    if (lim < a364566_cap) return;
    uint64_t cap = a364566_cap ? a364566_cap : 1024;
    while (cap <= lim) cap *= 2;
    unsigned char *comp = (unsigned char *)calloc((size_t)cap, 1);
    uint32_t *tab = (uint32_t *)malloc((size_t)cap * sizeof(uint32_t));
    comp[0] = 1;
    comp[1] = 1;
    for (uint64_t i = 2; i * i < cap; i++)
        if (!comp[i])
            for (uint64_t j = i * i; j < cap; j += i) comp[j] = 1;
    uint32_t c = 0;
    for (uint64_t i = 0; i < cap; i++) {
        if (!comp[i]) c++;
        tab[i] = c;
    }
    free(comp);
    free(a364566_pi);
    a364566_pi = tab;
    a364566_cap = cap;
}

static bool a364566_test(uint64_t k)
{
    if (k < 2) return false;
    a364566_ensure(k);
    uint64_t m = k, phi = 1, exp = 0;
    uint64_t maxp = 0;
    uint64_t e2 = 0;
    while ((m & 1) == 0) { m >>= 1; e2++; }
    if (e2 >= 1) {
        exp += e2 - 1;
        phi = (uint64_t)1 << (e2 - 1);
    }
    for (uint64_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            uint64_t e = 0;
            phi *= (p - 1);
            while (m % p == 0) {
                m /= p;
                e++;
                if (e > 1) phi *= p;
            }
            exp += e - 1;
            maxp = p;
        }
    }
    if (m > 1) {
        phi *= (m - 1);
        maxp = m;
    }
    if (maxp > 0) exp += (uint64_t)a364566_pi[maxp] - 1;
    if (exp >= 63) return false;
    return ((uint64_t)1 << exp) < phi;
}

seq_t a(idx_t n)
{
    static uint64_t *terms = NULL;
    static size_t cnt = 0, cap = 0;
    static uint64_t next_k = 1;
    while (cnt < (size_t)n) {
        if (a364566_test(next_k)) {
            if (cnt == cap) {
                cap = cap ? cap * 2 : 256;
                terms = (uint64_t *)realloc(terms, cap * sizeof(uint64_t));
            }
            terms[cnt++] = next_k;
        }
        next_k++;
    }
    return (seq_t)terms[n - 1];
}
