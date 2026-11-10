/*
 * A373380  Lexicographically earliest infinite sequence such that a(i) = a(j)
 *          => A373145(i) = A373145(j), A373362(i) = A373362(j), and
 *          A373364(i) = A373364(j), for all i, j >= 1.
 * https://oeis.org/A373380
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = restricted growth transform of the triple [gcd(x,y),
 *            gcd(x,z), gcd(y,z)], x=A001414(n), y=A003415(n), z=A276085(n);
 *            computed by scanning 1..n and numbering distinct triples by
 *            first occurrence
 * Range:     u8 n <= 705, u16 n <= 100000, u32 n <= 100000, u64 n <= 100000,
 *            u128 n <= 100000, big any n
 * Validated: standard, pass, 116 terms through n = 9771 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

static uint64_t A373380_gcd(uint64_t a, uint64_t b)
{
    while (b) { uint64_t t = a % b; a = b; b = t; }
    return a;
}

static uint64_t *A373380_primes = NULL;
static size_t A373380_pcnt = 0;
static uint64_t A373380_plimit = 0;

static void A373380_sieve(uint64_t limit)
{
    if (limit <= A373380_plimit) return;
    uint64_t L = limit;
    if (L < 2 * A373380_plimit) L = 2 * A373380_plimit;
    if (L < 100) L = 100;
    unsigned char *s = (unsigned char *)calloc((size_t)L + 1, 1);
    free(A373380_primes);
    A373380_primes = (uint64_t *)malloc(sizeof(uint64_t) * ((size_t)L / 2 + 10));
    A373380_pcnt = 0;
    for (uint64_t i = 2; i <= L; i++) {
        if (!s[i]) {
            A373380_primes[A373380_pcnt++] = i;
            for (uint64_t j = i * i; j <= L && i * i <= L; j += i) s[j] = 1;
        }
    }
    free(s);
    A373380_plimit = L;
}

#define A373380_NB 65536
static uint64_t *A373380_ids = NULL;
static uint64_t A373380_done = 0, A373380_cap = 0;
static uint64_t *A373380_t1 = NULL, *A373380_t2 = NULL, *A373380_t3 = NULL;
static size_t *A373380_nx = NULL;
static size_t A373380_D = 0, A373380_dcap = 0;
static size_t A373380_head[A373380_NB];
static int A373380_hinit = 0;

static uint64_t A373380_lookup(uint64_t g1, uint64_t g2, uint64_t g3)
{
    if (!A373380_hinit) {
        for (size_t i = 0; i < A373380_NB; i++) A373380_head[i] = (size_t)-1;
        A373380_hinit = 1;
    }
    uint64_t h = (g1 * 1000003ULL) ^ (g2 * 998244353ULL) ^ (g3 * 1469598103ULL);
    h ^= h >> 29;
    size_t b = (size_t)(h % A373380_NB);
    for (size_t k = A373380_head[b]; k != (size_t)-1; k = A373380_nx[k]) {
        if (A373380_t1[k] == g1 && A373380_t2[k] == g2 && A373380_t3[k] == g3)
            return (uint64_t)k + 1;
    }
    if (A373380_D == A373380_dcap) {
        A373380_dcap = A373380_dcap ? A373380_dcap * 2 : 1024;
        A373380_t1 = (uint64_t *)realloc(A373380_t1, sizeof(uint64_t) * A373380_dcap);
        A373380_t2 = (uint64_t *)realloc(A373380_t2, sizeof(uint64_t) * A373380_dcap);
        A373380_t3 = (uint64_t *)realloc(A373380_t3, sizeof(uint64_t) * A373380_dcap);
        A373380_nx = (size_t *)realloc(A373380_nx, sizeof(size_t) * A373380_dcap);
    }
    size_t k = A373380_D++;
    A373380_t1[k] = g1; A373380_t2[k] = g2; A373380_t3[k] = g3;
    A373380_nx[k] = A373380_head[b];
    A373380_head[b] = k;
    return (uint64_t)k + 1;
}

seq_t a(idx_t n)
{
    uint64_t N = (uint64_t)n;
    if (N < 1) return 0;
    if (N > A373380_done) {
        A373380_sieve(N);
        if (N > A373380_cap) {
            uint64_t nc = A373380_cap ? A373380_cap * 2 : 1024;
            if (nc < N) nc = N;
            A373380_ids = (uint64_t *)realloc(A373380_ids, sizeof(uint64_t) * (size_t)(nc + 1));
            A373380_cap = nc;
        }
        for (uint64_t i = A373380_done + 1; i <= N; i++) {
            uint64_t g1, g2, g3;
            if (i == 1) {
                g1 = g2 = g3 = 0;
            } else {
                uint64_t fp[64], fe[64];
                size_t fc = 0;
                uint64_t rem = i;
                for (size_t k = 0; k < A373380_pcnt; k++) {
                    uint64_t p = A373380_primes[k];
                    if (p * p > rem) break;
                    if (rem % p == 0) {
                        uint64_t e = 0;
                        while (rem % p == 0) { rem /= p; e++; }
                        fp[fc] = p; fe[fc] = e; fc++;
                    }
                }
                if (rem > 1) { fp[fc] = rem; fe[fc] = 1; fc++; }
                uint64_t x = 0, y = 0;
                for (size_t k = 0; k < fc; k++) {
                    x += fp[k] * fe[k];
                    y += (i / fp[k]) * fe[k];
                }
                /* z mod x and z mod y, z = A276085(i) */
                uint64_t zx = 0, zy = 0, px = 1 % x, py = 1 % y;
                size_t pk = 0;
                for (size_t k = 0; k < fc; k++) {
                    while (A373380_primes[pk] < fp[k]) {
                        px = (uint64_t)(((unsigned __int128)px * A373380_primes[pk]) % x);
                        py = (uint64_t)(((unsigned __int128)py * A373380_primes[pk]) % y);
                        pk++;
                    }
                    zx = (zx + (uint64_t)(((unsigned __int128)px * fe[k]) % x)) % x;
                    zy = (zy + (uint64_t)(((unsigned __int128)py * fe[k]) % y)) % y;
                }
                g1 = A373380_gcd(x, y);
                g2 = A373380_gcd(x, zx);
                g3 = A373380_gcd(y, zy);
            }
            A373380_ids[i] = A373380_lookup(g1, g2, g3);
        }
        A373380_done = N;
    }
    return (seq_t)A373380_ids[N];
}
