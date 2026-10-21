/*
 * A273287  Number of triples (x,y,z) with x,y,z in the set {0,...,n-1} such
 *          that x*y*z is a cube and x^3 + 2*y^3 + 3*z^3 == 7 (mod n).
 * https://oeis.org/A273287
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 72, u16 n <= 300, u32 n <= 300, u64 n <= 300,
 *            u128 n <= 300, big any n
 * Helpers:   factorize_int, gcd_int, mod_inv_ll
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation method: brute‑force search (moderate sieving).
   This implementation counts triples (x,y,z) ∈ {0,…,n−1} such that
   1) x*y*z is a perfect cube
   2) x^3 + 2*y^3 + 3*z^3 ≡ 7 (mod n)
   Triple counting is performed by: 1) handling all triples with at
   least one zero component explicitly, 2) enumerating all triples with
   x,y,z>0 by generating trivially all product cubes K^3 with
   K∈{1,…,n−1} and iterating over their factorizations. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/factorize_int.h"
#include "../utils/gcd_int.h"
#include "../utils/mod_inv_ll.h"

/* Helper: Euclidean gcd for int (n ≤ 10000). */

/* Build smallest prime factor (spf) array for numbers < limit. */
static void build_spf(int limit, int *spf) {
    for (int i = 0; i < limit; ++i) spf[i] = 0;
    if (limit > 1) spf[1] = 1;
    for (int i = 2; i < limit; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;
            if ((long long)i * i < limit) {
                for (int j = i * i; j < limit; j += i)
                    if (spf[j] == 0) spf[j] = i;
            }
        }
    }
}

/* Recursive divisor generation for prime powers; only keep divisors ≤ limit. */
static void gen_divisors_rec(int idx, int cnt, unsigned long long cur,
                             const int *primes, const int *exps,
                             unsigned long long limit, unsigned long long *divs,
                             size_t *divCnt) {
    if (idx == cnt) {
        divs[*divCnt] = cur;
        ++(*divCnt);
        return;
    }
    unsigned long long val = cur;
    for (int e = 0; e <= exps[idx]; ++e) {
        if (val > limit) break;
        gen_divisors_rec(idx + 1, cnt, val, primes, exps, limit, divs, divCnt);
        if (e < exps[idx]) {
            if (val > limit / (unsigned long long)primes[idx]) break;
            val *= (unsigned long long)primes[idx];
        }
    }
}

/* Main OEIS function. */
seq_t a(idx_t n) {
    /* local 64‑bit types for safety */
    const unsigned long long N = (unsigned long long)n;
    const unsigned long long target = 7ULL % N;
    const int n_int = (int)n;

    /* Pre‑compute cube residues and their counts. */
    seq_t *cube_mod = (seq_t *)malloc(sizeof(seq_t) * n);
    size_t *cube_counts = (size_t *)calloc((size_t)n, sizeof(size_t));
    for (int i = 0; i < n_int; ++i) {
        unsigned long long v = (unsigned long long)i * i % N * (unsigned long long)i % N;
        cube_mod[i] = (seq_t)v;
        ++cube_counts[(size_t)v];
    }

    /* ---------- count triples with at least one zero component ---------- */
    unsigned long long zeroCnt = 0;

    /* helper inverses for linear equations */
    int g3 = gcd_int(3, n_int);
    int nprime3 = n_int / g3;
    long long inv3 = mod_inv_ll(3 / g3, nprime3);

    int g2 = gcd_int(2, n_int);
    int nprime2 = n_int / g2;
    long long inv2 = mod_inv_ll(2 / g2, nprime2);

    /* S0: x = 0, count of (y,z). */
    unsigned long long S0 = 0;
    for (int u = 0; u < n_int; ++u) {
        size_t cntu = cube_counts[(size_t)u];
        if (!cntu) continue;
        long long t = (long long)target - 2LL * u;
        t %= n_int; if (t < 0) t += n_int;
        if (t % g3 != 0) continue;
        long long tdiv = t / g3;
        long long v0 = (inv3 * tdiv) % nprime3;
        size_t sumz = 0;
        for (int k = 0; k < g3; ++k) {
            int v = (int)(v0 + (long long)k * nprime3);
            sumz += cube_counts[(size_t)v];
        }
        S0 += (unsigned long long)cntu * sumz;
    }

    /* S1: y = 0, count of (x,z). */
    unsigned long long S1 = 0;
    for (int u = 0; u < n_int; ++u) {
        size_t cntu = cube_counts[(size_t)u];
        if (!cntu) continue;
        long long t = (long long)target - u;
        t %= n_int; if (t < 0) t += n_int;
        if (t % g3 != 0) continue;
        long long tdiv = t / g3;
        long long v0 = (inv3 * tdiv) % nprime3;
        size_t sumz = 0;
        for (int k = 0; k < g3; ++k) {
            int v = (int)(v0 + (long long)k * nprime3);
            sumz += cube_counts[(size_t)v];
        }
        S1 += (unsigned long long)cntu * sumz;
    }

    /* S2: z = 0, count of (x,y). */
    unsigned long long S2 = 0;
    for (int u = 0; u < n_int; ++u) {
        size_t cntu = cube_counts[(size_t)u];
        if (!cntu) continue;
        long long t = (long long)target - u;
        t %= n_int; if (t < 0) t += n_int;
        if (t % g2 != 0) continue;
        long long tdiv = t / g2;
        long long v0 = (inv2 * tdiv) % nprime2;
        size_t sumz = 0;
        for (int k = 0; k < g2; ++k) {
            int v = (int)(v0 + (long long)k * nprime2);
            sumz += cube_counts[(size_t)v];
        }
        S2 += (unsigned long long)cntu * sumz;
    }

    /* Double intersections. */
    size_t X0 = cube_counts[(size_t)target];               /* x=0,y=0 */
    size_t Y0;
    if (target % g2 != 0) Y0 = 0;
    else {
        long long tdiv = target / g2;
        long long u0 = (inv2 * tdiv) % nprime2;
        Y0 = 0;
        for (int k = 0; k < g2; ++k) {
            int u = (int)(u0 + (long long)k * nprime2);
            Y0 += cube_counts[(size_t)u];
        }
    }
    size_t Z0;
    if (target % g3 != 0) Z0 = 0;
    else {
        long long tdiv = target / g3;
        long long u0 = (inv3 * tdiv) % nprime3;
        Z0 = 0;
        for (int k = 0; k < g3; ++k) {
            int u = (int)(u0 + (long long)k * nprime3);
            Z0 += cube_counts[(size_t)u];
        }
    }
    size_t triple = (target == 0ULL) ? 1 : 0;

    zeroCnt = S0 + S1 + S2 - (X0 + Y0 + Z0) + triple;

    /* ---------- count triples with all components > 0 ---------- */
    unsigned long long allNonZeroCnt = 0;
    if (n_int > 1) {
        /* Smallest prime factor array for factorization. */
        int *spf = (int *)malloc(sizeof(int) * n_int);
        build_spf(n_int, spf);

        /* Temporary arrays for factorization and divisors. */
        int primes[20], exps[20], pfCnt;
        unsigned long long divs[2000];
        size_t divCnt;

        for (int K = 1; K < n_int; ++K) {
            /* Factorize K, prepare exponents *3 for K^3. */
            factorize_int(K, spf, primes, exps, &pfCnt);
            for (int i = 0; i < pfCnt; ++i) exps[i] *= 3;

            /* Generate all divisors of K^3 not exceeding n-1. */
            divCnt = 0;
            gen_divisors_rec(0, pfCnt, 1ULL, primes, exps,
                             (unsigned long long)(n_int - 1), divs, &divCnt);

            unsigned long long K3 = (unsigned long long)K * (unsigned long long)K *
                                    (unsigned long long)K;
            for (size_t i = 0; i < divCnt; ++i) {
                unsigned long long x = divs[i];
                for (size_t j = 0; j < divCnt; ++j) {
                    unsigned long long y = divs[j];
                    unsigned long long xy = x * y;
                    if (xy == 0) continue;
                    if (K3 % xy != 0) continue;
                    unsigned long long z = K3 / xy;
                    if (z > (unsigned long long)(n_int - 1)) continue;

                    seq_t val = (cube_mod[x] + 2ULL * cube_mod[y] +
                                 3ULL * cube_mod[z]) % N;
                    if (val == target) ++allNonZeroCnt;
                }
            }
        }
        free(spf);
    }

    free(cube_mod);
    free(cube_counts);

    unsigned long long result = zeroCnt + allNonZeroCnt;
    return (seq_t)result;
}
