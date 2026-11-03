/*
 * A356165  a(n) = n minus the smallest positive k such that n divides
 *          k*A003961(k), where A003961 is fully multiplicative with a(p) =
 *          nextprime(p).
 * https://oeis.org/A356165
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 269, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 107 terms through n = 9604 (2026-11-03)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A356165: n minus the least k >= 1 with n | k A003961(k), A003961 being fully
 * multiplicative with p -> nextprime(p); k = n always works. k A003961(k) is
 * multiplicative with p^e -> (p nextprime(p))^e, taken modulo n in 128-bit
 * steps. A smallest-prime-factor table and the next prime of each prime grow
 * with n (the original's tables stopped at 40000). */

#include <stdint.h>
#include <stdlib.h>

static uint32_t *spf = NULL, *nextp = NULL;
static uint64_t lim = 0;

static void build(uint64_t L)
{
    free(spf);
    free(nextp);
    spf = (uint32_t *)calloc((size_t)L + 1, sizeof *spf);
    nextp = (uint32_t *)calloc((size_t)L + 1, sizeof *nextp);
    if (!spf || !nextp) abort();
    for (uint64_t i = 2; i <= L; ++i)
        if (!spf[i])
            for (uint64_t j = i; j <= L; j += i)
                if (!spf[j]) spf[j] = (uint32_t)i;
    uint64_t prev = 0;
    for (uint64_t i = 2; i <= L; ++i)
        if (spf[i] == i) {
            if (prev) nextp[prev] = (uint32_t)i;
            prev = i;
        }
    lim = L;                              /* nextp is set for primes below the last one */
}

static uint64_t mulmod(uint64_t a, uint64_t b, uint64_t m)
{
    return (uint64_t)((unsigned __int128)a * b % m);
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    const uint64_t N = (uint64_t)n;
    if (N + 1000 > lim) {                 /* room for nextprime of every p <= n */
        uint64_t L = lim ? 2 * lim : 1 << 16;
        while (L < N + 1000) L *= 2;
        build(L);
    }
    for (uint64_t k = 1; k <= N; ++k) {
        uint64_t g = 1 % N, t = k;
        while (t > 1 && g) {
            uint64_t p = spf[t];
            uint64_t b = mulmod(p % N, nextp[p] % N, N);
            while (t % p == 0) {
                t /= p;
                g = mulmod(g, b, N);
            }
        }
        if (g == 0) return (seq_t)(N - k);
    }
    return (seq_t)0;
}
