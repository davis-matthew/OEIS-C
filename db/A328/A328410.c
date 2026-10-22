/*
 * A328410  Smallest m such that (Z/mZ)* = C_2 X C_(2n), or 0 if no such m
 *          exists, where (Z/mZ)* is the multiplicative group of integers
 *          modulo m.
 * https://oeis.org/A328410
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 6560, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Smallest m such that (Z/mZ)* = C_2 × C_(2n).  The function
    scans all factorisations of n into coprime parts a,b with a*b=n.
    For each part it fetches the smallest odd prime power p^k with
    φ(p^k) = 2a (respectively 2b).  The candidates are:
    - 8*n        (n a power of two)
    - 4 * p^k    (φ(p^k)=2n)
    - p1^e1 * p2^e2 (with φ(p1^e1)=2a, φ(p2^e2)=2b, a*b=n, gcd(a,b)=1,
      and the primes are distinct)
    The minimum of all valid candidates is returned; 0 if none. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define MAXPHI 20000   /* 2*n for n <= 10000 */

static bool built = false;
static uint64_t min_pow[MAXPHI + 1] = {0};
static uint32_t min_prime[MAXPHI + 1] = {0};
static uint32_t primes[3000];
static size_t primes_cnt = 0;

/* Euclidean gcd for int */
static int int_gcd(int a, int b)
{
    while (b) { int t = a % b; a = b; b = t; }
    return a;
}

/* Heuristic floor of sqrt for small ints */
static size_t int_sqrt(size_t n)
{
    size_t s = 0;
    while (s * s <= n) ++s;
    return s - 1;
}

static void init(void)
{
    if (built) return;
    /* Simple sieve for primes up to MAXPHI */
    bool comp[MAXPHI + 1] = {0};
    primes_cnt = 0;
    for (size_t i = 2; i <= MAXPHI; ++i) {
        if (!comp[i]) {
            if (primes_cnt < sizeof(primes)/sizeof(primes[0]))
                primes[primes_cnt++] = (uint32_t)i;
            if (i * i <= MAXPHI) {
                for (size_t j = i * i; j <= MAXPHI; j += i) comp[j] = true;
            }
        }
    }

    /* For each odd prime power compute φ and keep minimal power */
    for (size_t pi = 0; pi < primes_cnt; ++pi) {
        uint64_t p = primes[pi];
        uint64_t pow = p;
        uint64_t phi = p - 1;          /* φ(p)   */
        while (phi <= MAXPHI) {
            if (min_pow[phi] == 0 || pow < min_pow[phi]) {
                min_pow[phi] = pow;
                min_prime[phi] = (uint32_t)p;
            }
            if (pow > UINT64_MAX / p) break;
            pow *= p;
            /* φ(p^k) = p^(k-1)*(p-1) = (pow/p)*(p-1) */
            phi = (pow / p) * (p - 1);
        }
    }
    built = true;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    init();

    uint64_t N = (uint64_t)n;
    uint64_t best = 0;   /* 0 means not found yet */

    /* Case 1: 8 * n, only if n is a power of two */
    if ((N & (N - 1)) == 0) {
        uint64_t m = (uint64_t)8 * N;
        best = m;
    }

    /* Case 2: 4 * p^k where φ(p^k) = 2n */
    uint64_t twon = 2 * N;
    if (twon <= MAXPHI) {
        uint64_t pw = min_pow[twon];
        if (pw && min_prime[twon] != 2) {              /* odd prime power */
            uint64_t m = 4ULL * pw;
            if (best == 0 || m < best) best = m;
        }
    }

    /* Case 3: product of two odd prime powers */
    size_t sq = int_sqrt(N);
    for (size_t i = 1; i <= sq; ++i) {
        if (N % i != 0) continue;
        uint64_t a1 = (uint64_t)i;
        uint64_t b1 = N / i;
        if (int_gcd((int)a1, (int)b1) != 1) continue;

        uint64_t targetA = 2 * a1;
        uint64_t targetB = 2 * b1;
        if (targetA > MAXPHI || targetB > MAXPHI) continue;
        uint64_t powA = min_pow[targetA];
        uint64_t powB = min_pow[targetB];
        if (!powA || !powB) continue;
        if (min_prime[targetA] == 2 || min_prime[targetB] == 2) continue;
        if (min_prime[targetA] == min_prime[targetB]) continue;   /* same prime */

        uint64_t m = powA * powB;
        if (best == 0 || m < best) best = m;
    }

    return (seq_t)(best == 0 ? 0 : best);
}
