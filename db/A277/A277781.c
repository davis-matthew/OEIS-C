/*
 * A277781  a(n) is the least k > n such that n*k or n*k^2 is a cube.
 * https://oeis.org/A277781
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = min{ k>n | n*k is a cube or n*k^2 is a cube }
 * Range:     u8 n <= 16, u16 n <= 6064, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floorCubeRoot
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/floorCubeRoot.h"

/* ---------- Helper: factorisation of a 64‑bit integer ---------- */
static void factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt)
{
    size_t i = 0;
    if (n < 2) { *cnt = 0; return; }
    for (uint64_t p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            uint64_t e = 0;
            while (n % p == 0) {
                n /= p;
                ++e;
            }
            pr[i] = p;
            ex[i] = e;
            ++i;
        }
    }
    if (n > 1) { pr[i] = n; ex[i] = 1; ++i; }
    *cnt = i;
}

/* ---------- Main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;           /* safety for out‑of‑range */

    /* Convert index to seq_t for later arithmetic */
    seq_t sn = (seq_t)n;

    /* Factorise n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t pcnt = 0;
    factor_u64(n, primes, exps, &pcnt);

    /* Minimal k for n*k is a cube */
    seq_t k1 = 1;
    /* Minimal k for n*k^2 is a cube */
    seq_t k2 = 1;

    for (size_t i = 0; i < pcnt; ++i) {
        int r = (int)(exps[i] % 3);
        int need1 = (3 - r) % 3;   /* for n*k */
        int need2 = r;             /* for n*k^2 */

        for (int j = 0; j < need1; ++j) k1 *= (seq_t)primes[i];
        for (int j = 0; j < need2; ++j) k2 *= (seq_t)primes[i];
    }

    /* Find smallest t such that k = k_min * t^3 > n */
    seq_t t1 = floorCubeRoot(sn / k1) + 1;
    seq_t t2 = floorCubeRoot(sn / k2) + 1;

    seq_t t1s = t1 * t1;
    seq_t t1c = t1s * t1;
    seq_t t2s = t2 * t2;
    seq_t t2c = t2s * t2;

    seq_t cand1 = k1 * t1c;
    seq_t cand2 = k2 * t2c;

    return (cand1 < cand2) ? cand1 : cand2;
}
