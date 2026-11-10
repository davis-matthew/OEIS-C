/*
 * A353352  Number of divisors d of n for which A048675(d) is a multiple of 3.
 * https://oeis.org/A353352
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} [A048675(d) == 0 (mod 3)], computed from the
 *            factorization n = Prod p_i^e_i: A048675(d) mod 3 = Sum f_i *
 *            2^(PrimePi(p_i)-1) mod 3 over d = Prod p_i^f_i, counted with a
 *            residue-class DP over exponents (2^k mod 3 = 1 if k even, 2 if k
 *            odd)
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9539 (2026-11-10)
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"

/* parity of PrimePi(x) for each prime factor, via a sieve up to the largest prime factor */
static unsigned char *A353352_comp = NULL;
static uint64_t A353352_lim = 0;

static void A353352_ensure_sieve(uint64_t lim)
{
    uint64_t i, j;
    if (lim <= A353352_lim && A353352_comp != NULL) return;
    if (lim < 1024) lim = 1024;
    if (lim < 2 * A353352_lim) lim = 2 * A353352_lim;
    free(A353352_comp);
    A353352_comp = (unsigned char *)calloc((size_t)(lim + 1), 1);
    A353352_comp[0] = 1;
    A353352_comp[1] = 1;
    for (i = 2; i * i <= lim; ++i) {
        if (!A353352_comp[i]) {
            for (j = i * i; j <= lim; j += i) A353352_comp[j] = 1;
        }
    }
    A353352_lim = lim;
}

static uint64_t A353352_prime_pi(uint64_t p)
{
    uint64_t c = 0, k;
    A353352_ensure_sieve(p);
    for (k = 2; k <= p; ++k) if (!A353352_comp[k]) ++c;
    return c;
}

seq_t a(idx_t n)
{
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    uint64_t cur[3] = {1, 0, 0};
    size_t i;
    uint64_t j;
    int r;

    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    for (i = 0; i < cnt; ++i) {
        uint64_t pi = A353352_prime_pi(primes[i]);
        /* 2^(pi-1) mod 3: 1 if pi-1 even, else 2 */
        int w = (((pi - 1) & 1) == 0) ? 1 : 2;
        uint64_t nxt[3] = {0, 0, 0};
        for (j = 0; j <= exps[i]; ++j) {
            int shift = (int)(((j % 3) * (uint64_t)w) % 3);
            for (r = 0; r < 3; ++r) {
                nxt[(r + shift) % 3] += cur[r];
            }
        }
        cur[0] = nxt[0];
        cur[1] = nxt[1];
        cur[2] = nxt[2];
    }
    return (seq_t)cur[0];
}
