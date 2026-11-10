/*
 * A359640  a(n) is the least odd prime not in A001359 such that all
 *          subsequent composites in the gap up to the next prime have exactly
 *          n odd prime factors, counted with multiplicity.
 * https://oeis.org/A359640
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, hard, more
 * Formula:   search. a(n) = x - 2 for the least odd x with bigomega(x) = n
 *            such that x - 2 is prime and every composite from x - 1 up to
 *            the next prime has exactly n odd prime factors (with
 *            multiplicity). Candidates x are generated as odd n-almost-primes
 *            in doubling windows [lo, 2*lo)
 * Range:     u8 none, u16 n <= 3, u32 n <= 6, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Validated: partial, pass, 6 terms through n = 7 (2026-11-10)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/is_prime_u64.h"

static uint32_t *a359640_P = NULL;   /* odd primes 3,5,7,... */
static size_t a359640_np = 0;
static uint64_t a359640_lim = 0;
static uint64_t a359640_lo, a359640_hi, a359640_best;
static int a359640_n;
static uint64_t a359640_cache[64];

static void a359640_sieve(uint64_t L) {
    unsigned char *c;
    uint32_t *l;
    size_t cnt = 0, cap;
    uint64_t i, j;
    if (L <= a359640_lim) return;
    if (L < 2 * a359640_lim) L = 2 * a359640_lim;
    if (L < ((uint64_t)1 << 21)) L = (uint64_t)1 << 21;
    c = (unsigned char *)calloc((size_t)(L + 1), 1);
    cap = (size_t)(L / 8 + 1024);
    l = (uint32_t *)malloc(cap * sizeof(uint32_t));
    for (i = 3; i <= L; i += 2) {
        if (!c[i]) {
            if (cnt == cap) {
                cap *= 2;
                l = (uint32_t *)realloc(l, cap * sizeof(uint32_t));
            }
            l[cnt++] = (uint32_t)i;
            if (i <= L / i)
                for (j = i * i; j <= L; j += 2 * i) c[j] = 1;
        }
    }
    free(c);
    free(a359640_P);
    a359640_P = l;
    a359640_np = cnt;
    a359640_lim = L;
}

/* true if p^k > m */
static bool a359640_pwgt(uint64_t p, int k, uint64_t m) {
    uint64_t t = 1;
    int j;
    for (j = 0; j < k; j++) {
        if (t > m / p) return true;
        t *= p;
    }
    return t > m;
}

#define A359640_STEP(p, q)                                              \
    if (m % p == 0) {                                                   \
        do { m /= p; need--; } while (m % p == 0);                      \
        if (need < 0) return false;                                     \
    }                                                                   \
    if (need == 0) return m == 1;                                       \
    if (m == 1) return false;                                           \
    if (a359640_pwgt(q, need, m)) return false;                         \
    if (need == 1 && m / q < q) return true;

/* does the odd part of m have exactly n prime factors (with multiplicity)? */
static bool a359640_chk(uint64_t m, int n) {
    int need = n;
    size_t k;
    bool test = true;
    if (m == 0) return false;
    while ((m & 1) == 0) m >>= 1;
    if (need == 0) return m == 1;
    if (m == 1) return false;
    if (a359640_pwgt(3, need, m)) return false;
    A359640_STEP(3, 5)
    A359640_STEP(5, 7)
    A359640_STEP(7, 11)
    A359640_STEP(11, 13)
    A359640_STEP(13, 17)
    A359640_STEP(17, 19)
    A359640_STEP(19, 23)
    A359640_STEP(23, 29)
    A359640_STEP(29, 31)
    A359640_STEP(31, 37)
    for (k = 10;; k++) {
        uint64_t pr;
        if (need == 1) return is_prime_u64(m);
        if (test) {
            if (is_prime_u64(m)) return false;
            test = false;
        }
        while (k >= a359640_np) a359640_sieve(a359640_lim * 2);
        pr = a359640_P[k];
        if (a359640_pwgt(pr, need, m)) return false;
        if (m % pr == 0) {
            do { m /= pr; need--; } while (m % pr == 0);
            if (need < 0) return false;
            if (need == 0) return m == 1;
            if (m == 1) return false;
            test = true;
        }
    }
}

static bool a359640_test(uint64_t x, int n) {
    uint64_t m;
    if (!a359640_chk(x + 1, n)) return false;
    if (!a359640_chk(x - 1, n)) return false;
    if (x < 5 || !is_prime_u64(x - 2)) return false;
    m = x + 2;
    while (!is_prime_u64(m)) {
        if (!a359640_chk(m, n)) return false;
        m++;
    }
    return true;
}

static void a359640_dfs(uint64_t prod, size_t si, int rem) {
    size_t k;
    if (rem == 1) {
        uint64_t qmin = (a359640_lo + prod - 1) / prod;
        size_t lo = si, hi = a359640_np;
        if (si >= a359640_np) return;
        while (lo < hi) {
            size_t mid = lo + (hi - lo) / 2;
            if ((uint64_t)a359640_P[mid] < qmin) lo = mid + 1; else hi = mid;
        }
        for (k = lo; k < a359640_np; k++) {
            uint64_t q = a359640_P[k];
            uint64_t x;
            if (q > (a359640_hi - 1) / prod) break;
            x = prod * q;
            if (a359640_test(x, a359640_n)) {
                a359640_best = x;
                a359640_hi = x;
                break;
            }
        }
        return;
    }
    for (k = si; k < a359640_np; k++) {
        uint64_t p = a359640_P[k];
        if (a359640_pwgt(p, rem, (a359640_hi - 1) / prod)) break;
        a359640_dfs(prod * p, k, rem - 1);
    }
}

seq_t a(idx_t n) {
    int nn, i;
    uint64_t lo, p3;
    if (n < 2 || n > 38) return 0;
    nn = (int)n;
    if (a359640_cache[nn]) return (seq_t)a359640_cache[nn];
    lo = 1;
    for (i = 0; i < nn; i++) lo *= 3;
    p3 = lo / 3;
    for (;;) {
        uint64_t hi;
        if (lo > ((uint64_t)1 << 62)) return 0;
        hi = lo * 2;
        a359640_sieve((hi - 1) / p3 + 1);
        a359640_lo = lo;
        a359640_hi = hi;
        a359640_best = 0;
        a359640_n = nn;
        a359640_dfs(1, 0, nn);
        if (a359640_best) {
            a359640_cache[nn] = a359640_best - 2;
            return (seq_t)(a359640_best - 2);
        }
        lo = hi;
    }
}
