/*
 * A328496  Triangular array read by rows: for 1 <= k <= n, T(n,k) is the
 *          number of x in [1,k] such that gcd(n,x) > 1 and gcd(k,x) > 1.
 * https://oeis.org/A328496
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = k - C(n,k) - φ(k) + C(lcm(n,k),k) where C(m, x) = #
 *            {1≤t≤x : gcd(m,t)=1} and φ(k) is Euler's totient function. The
 *            sequence is read by rows: a(n)=T(r,c) where r is the unique
 *            integer with r(r+1)/2 ≥ n and c = n - (r-1)*r/2
 * Range:     u8 n <= 10011, u16 n <= 10011, u32 n <= 10011, u64 n <= 10011,
 *            u128 n <= 10011, big any n
 * Validated: standard, pass, 107 terms through n = 9638 (2026-11-01)
 * Helpers:   gcd_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include "../utils/gcd_u64.h"

/* Count numbers up to k coprime to a given set of primes.
   primes[] contains distinct primes; pcnt=number of primes.  */
static inline uint64_t count_coprime(uint64_t k, const uint16_t *pr, uint8_t pcnt) {
    uint64_t ans = 0;
    uint32_t total = 1u << pcnt;          /* pcnt ≤ 5, 2^5 = 32 */
    for (uint32_t mask = 0; mask < total; ++mask) {
        uint64_t prod = 1;
        uint8_t bits = 0;
        for (uint8_t i = 0; i < pcnt; ++i) {
            if (mask & (1u << i)) {
                prod *= pr[i];
                ++bits;
                if (prod > k) break;
            }
        }
        if (prod > k) continue;
        int8_t mu = (bits & 1) ? -1 : 1;   /* μ(d) = (-1)^{bits} for square‑free d */
        ans += mu * (k / prod);
    }
    return ans;
}

/* Merge prime factor lists of two numbers into primes_uni[10] (max 10 distinct primes) */
static inline uint8_t merge_primes(const uint16_t *pa, uint8_t a, const uint16_t *pb, uint8_t b, uint16_t *uni) {
    uint8_t i = 0, j = 0, k = 0;
    while (i < a && j < b) {
        if (pa[i] < pb[j]) {
            uni[k++] = pa[i++];
        } else if (pa[i] > pb[j]) {
            uni[k++] = pb[j++];
        } else {
            uni[k++] = pa[i];
            ++i; ++j;
        }
    }
    while (i < a) uni[k++] = pa[i++];
    while (j < b) uni[k++] = pb[j++];
    return k;               /* number of distinct primes in union */
}

/* Pre‑computation parameters */
#define MAX_PRE 20000          /* maximum n (row) expected */
#define MAX_FACTS 5            /* maximum distinct prime factors for numbers ≤ MAX_PRE */

/* Static storage for pre‑computed data */
static bool initialized = false;
static uint32_t phi[MAX_PRE + 1];
static uint16_t primes[MAX_PRE + 1][MAX_FACTS];
static uint8_t  pcnt[MAX_PRE + 1];
static uint32_t spf[MAX_PRE + 1];

/* Euclidean GCD */

/* Determine row number from index (1‑based).  */
static inline uint64_t row_from_index(uint64_t idx) {
    /* using long double for better accuracy for large idx */
    long double v = sqrtl(8.0L * idx + 1.0L);
    uint64_t r = (uint64_t)((v - 1.0L) / 2.0L);
    /* adjust because of possible rounding errors */
    while (r * (r + 1) / 2 < idx) ++r;
    while ((r - 1) * r / 2 >= idx) --r;
    return r;
}

/* One‑time pre‑computation of φ, prime factor lists and smallest prime factor.  */
static void init(void) {
    if (initialized) return;
    /* Smallest prime factor */
    for (uint32_t i = 0; i <= MAX_PRE; ++i) spf[i] = i;
    for (uint32_t i = 2; i * i <= MAX_PRE; ++i) {
        if (spf[i] == i) {
            for (uint32_t j = i * i; j <= MAX_PRE; j += i)
                if (spf[j] == j) spf[j] = i;
        }
    }
    /* Euler's Totient */
    for (uint32_t i = 0; i <= MAX_PRE; ++i) phi[i] = i;
    for (uint32_t p = 2; p <= MAX_PRE; ++p) {
        if (spf[p] == p) {           /* p is prime */
            for (uint32_t j = p; j <= MAX_PRE; j += p)
                phi[j] -= phi[j] / p;
        }
    }
    /* Distinct prime factor lists */
    pcnt[0] = pcnt[1] = 0;
    for (uint32_t n = 2; n <= MAX_PRE; ++n) {
        uint32_t tmp = n;
        uint8_t idx = 0;
        while (tmp > 1) {
            uint32_t p = spf[tmp];
            if (idx == 0 || primes[n][idx - 1] != p) {
                primes[n][idx++] = (uint16_t)p;
            }
            while (tmp % p == 0) tmp /= p;
        }
        pcnt[n] = idx;
    }
    initialized = true;
}

/* Main sequence function required by the framework. */
seq_t a(idx_t n) {
    init();

    /* Map sequence index to (row,column) */
    uint64_t r = row_from_index((uint64_t)n);          /* row number, 1‑based */
    uint64_t c = (uint64_t)n - (r - 1) * r / 2;        /* column number (k) */

    /* φ(c) */
    uint32_t phi_c = phi[(uint32_t)c];

    /* C(n,r), i.e., numbers ≤c coprime to r */
    uint64_t Cop_r = count_coprime(c,
                                   primes[(uint32_t)r],
                                   pcnt[(uint32_t)r]);

    /* lcm(r,c) and its distinct prime set = union of primes of r and c */
    uint64_t g = gcd_u64((uint64_t)r, c);
    uint64_t lcm = (r / g) * c;   /* lcm fits in 64‑bit for given limits */

    /* Prepare union of prime sets */
    uint16_t uni[10];
    uint8_t kup = merge_primes(primes[(uint32_t)r], pcnt[(uint32_t)r],
                               primes[(uint32_t)c], pcnt[(uint32_t)c],
                               uni);
    /* C(lcm, c) */
    uint64_t Cop_lcm = count_coprime(c, uni, kup);

    /* Compute T(r,c) = c - Cop_r - φ(c) + Cop_lcm */
    seq_t res = (seq_t)(c - Cop_r - phi_c + Cop_lcm);
    return res;
}
