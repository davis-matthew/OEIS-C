/*
 * A264887  Numbers in A007504 such that omega(a(n)) = Omega(a(n)) = 4.
 * https://oeis.org/A264887
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search: a(n) = n-th partial sum of primes A007504(k) that is a
 *            product of exactly four distinct primes (omega = Omega = 4);
 *            each sum is tested by pruned trial division plus a Miller-Rabin
 *            test on the last cofactor
 * Range:     u8 none, u16 n <= 8, u32 n <= 3681, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 46 terms through n = 9756 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define A264887_LIM 4000000u

static uint32_t *a264887_pr = NULL;
static size_t a264887_np = 0;
static uint64_t *a264887_t = NULL;
static size_t a264887_nt = 0, a264887_cap = 0;
static size_t a264887_k = 0;
static uint64_t a264887_sum = 0;

static uint64_t a264887_mulmod(uint64_t a, uint64_t b, uint64_t m) {
    return (uint64_t)(((unsigned __int128)a * b) % m);
}

static uint64_t a264887_powmod(uint64_t b, uint64_t e, uint64_t m) {
    uint64_t r = 1;
    b %= m;
    while (e) {
        if (e & 1) r = a264887_mulmod(r, b, m);
        b = a264887_mulmod(b, b, m);
        e >>= 1;
    }
    return r;
}

static bool a264887_isprime(uint64_t n) {
    static const uint64_t bases[12] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};
    uint64_t d;
    int s = 0, i, r;
    if (n < 2) return false;
    for (i = 0; i < 12; i++) {
        if (n == bases[i]) return true;
        if (n % bases[i] == 0) return false;
    }
    d = n - 1;
    while ((d & 1) == 0) { d >>= 1; s++; }
    for (i = 0; i < 12; i++) {
        uint64_t x = a264887_powmod(bases[i], d, n);
        if (x == 1 || x == n - 1) continue;
        for (r = 1; r < s; r++) {
            x = a264887_mulmod(x, x, n);
            if (x == n - 1) break;
        }
        if (r == s) return false;
    }
    return true;
}

static void a264887_init(void) {
    unsigned char *c = (unsigned char *)calloc(A264887_LIM + 1, 1);
    size_t cnt = 0;
    uint32_t i, j;
    for (i = 2; (uint64_t)i * i <= A264887_LIM; i++)
        if (!c[i]) for (j = i * i; j <= A264887_LIM; j += i) c[j] = 1;
    for (i = 2; i <= A264887_LIM; i++) if (!c[i]) cnt++;
    a264887_pr = (uint32_t *)malloc(cnt * sizeof(uint32_t));
    cnt = 0;
    for (i = 2; i <= A264887_LIM; i++) if (!c[i]) a264887_pr[cnt++] = i;
    a264887_np = cnt;
    free(c);
}

/* true iff s is a product of exactly four distinct primes */
static bool a264887_check(uint64_t s) {
    int cnt = 0;
    uint64_t m = s;
    size_t i;
    for (i = 0; i < a264887_np; i++) {
        uint64_t p = a264887_pr[i];
        int need = 4 - cnt;
        if (need == 0) return m == 1;
        if (need == 1) return m > 1 && a264887_isprime(m);
        if (need == 2 && p > m / p) return false;
        if (need == 3 && p > m / p / p) return false;
        if (need == 4 && p > m / p / p / p) return false;
        if (m % p == 0) {
            m /= p;
            if (m % p == 0) return false;
            cnt++;
        }
    }
    return false;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if (!a264887_pr) a264887_init();
    while (a264887_nt < (size_t)n && a264887_k < a264887_np) {
        a264887_sum += a264887_pr[a264887_k++];
        if (a264887_check(a264887_sum)) {
            if (a264887_nt == a264887_cap) {
                a264887_cap = a264887_cap ? a264887_cap * 2 : 256;
                a264887_t = (uint64_t *)realloc(a264887_t, a264887_cap * sizeof(uint64_t));
            }
            a264887_t[a264887_nt++] = a264887_sum;
        }
    }
    if ((size_t)n > a264887_nt) return 0;
    return (seq_t)a264887_t[n - 1];
}
