/*
 * A270433  a(n) = number of terms A270431 <= n; least monotonic left inverse
 *          of A270431.
 * https://oeis.org/A270433
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 0, a(n) = a(n-1) + ((A048673(n) - A064216(n)) mod 2);
 *            only parities are needed (prefix sums, memoized)
 * Range:     u8 n <= 509, u16 n <= 35000, u32 n <= 35000, u64 n <= 35000,
 *            u128 n <= 35000, big any n
 * Validated: standard, pass, 104 terms through n = 8889 (2026-11-07)
 * Helpers:   factor_u64, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/next_prime.h"

static uint64_t *A270433_tab = NULL;
static uint64_t A270433_have = 0;   /* tab[1..have] valid */
static uint64_t A270433_cap = 0;

/* parity of A048673(k) - A064216(k) */
static unsigned A270433_diff(uint64_t k)
{
    uint64_t primes[64], exps[64];
    size_t cnt = 0, i;
    uint64_t x = 1; /* A003961(k) mod 4 */
    if (k > 1) {
        factor_u64(k, primes, exps, &cnt);
        for (i = 0; i < cnt; ++i) {
            uint64_t r = next_prime(primes[i]) % 4;
            uint64_t e;
            for (e = 0; e < exps[i]; ++e)
                x = (x * r) % 4;
        }
    }
    unsigned pa = (x % 4 == 1) ? 1u : 0u;      /* parity of (A003961+1)/2 */
    unsigned pb = (k % 3 != 2) ? 1u : 0u;      /* A064216(k) even iff 3 | 2k-1 */
    return pa ^ pb;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    if (n + 1 > A270433_cap) {
        uint64_t nc = (uint64_t)n + 1024;
        uint64_t *p = (uint64_t *)realloc(A270433_tab, nc * sizeof(uint64_t));
        if (!p) return (seq_t)0;
        A270433_tab = p;
        A270433_cap = nc;
    }
    if (A270433_have == 0) {
        A270433_tab[1] = 0;
        A270433_have = 1;
    }
    while (A270433_have < (uint64_t)n) {
        uint64_t k = A270433_have + 1;
        A270433_tab[k] = A270433_tab[k - 1] + A270433_diff(k);
        A270433_have = k;
    }
    return (seq_t)A270433_tab[n];
}
