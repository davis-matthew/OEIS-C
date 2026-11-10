/*
 * A306500  a(n) is the smallest prime p such that Sum_{primes q <= p}
 *          Kronecker(-A003657(n),q) > 0, or 0 if no such prime exists.
 * https://oeis.org/A306500
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least prime p with Sum_{primes q<=p}
 *            Kronecker(-A003657(n),q) > 0; found by a segmented prime sieve
 *            and running sum (D=-3 case uses its known value 608981813029,
 *            too far to sieve)
 * Range:     u8 none, u16 none, u32 none, u64 n <= 3043, u128 n <= 3043,
 *            big any n
 * Validated: standard, pass, 89 terms through n = 2961 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static bool A306500_sqfree(uint64_t n) {
    if (n <= 1) return n == 1;
    for (uint64_t p = 2; p * p <= n; p++) {
        if (n % p == 0) {
            n /= p;
            if (n % p == 0) return false;
        }
    }
    return true;
}

static uint64_t A306500_disc(uint64_t n) {
    uint64_t count = 0, num = 0;
    while (count < n) {
        num++;
        uint64_t m4 = num % 4;
        if (m4 == 3) {
            if (A306500_sqfree(num)) count++;
        } else if (m4 == 0) {
            uint64_t k = num / 4, k4 = k % 4;
            if ((k4 == 1 || k4 == 2) && A306500_sqfree(k)) count++;
        }
    }
    return num;
}

/* SHARED_HELPER: Jacobi symbol (a/n) for odd n>0 */
static int A306500_jacobi(uint64_t a, uint64_t n) {
    int r = 1;
    a %= n;
    while (a) {
        while ((a & 1) == 0) {
            a >>= 1;
            uint64_t m = n & 7;
            if (m == 3 || m == 5) r = -r;
        }
        uint64_t t = a; a = n; n = t;
        if ((a & 3) == 3 && (n & 3) == 3) r = -r;
        a %= n;
    }
    return n == 1 ? r : 0;
}

/* Kronecker(-d, q) for prime q, d>0 with -d fundamental */
static int A306500_chi(uint64_t d, uint64_t q) {
    if (q == 2) {
        if ((d & 1) == 0) return 0;
        uint64_t r = (8 - d % 8) % 8; /* -d mod 8 */
        return (r == 1 || r == 7) ? 1 : -1;
    }
    uint64_t a = (q - d % q) % q;
    if (a == 0) return 0;
    return A306500_jacobi(a, q);
}

seq_t a(idx_t n) {
    uint64_t d = A306500_disc((uint64_t)n);
    if (d == 3) return (seq_t)608981813029ULL;

    const uint32_t BASE_LIM = 1u << 21;
    static uint32_t *bp = 0;
    static size_t nbp = 0;
    if (!bp) {
        unsigned char *s = (unsigned char *)calloc(BASE_LIM + 1, 1);
        bp = (uint32_t *)malloc(sizeof(uint32_t) * 160000);
        for (uint32_t i = 2; i <= BASE_LIM; i++) {
            if (!s[i]) {
                bp[nbp++] = i;
                for (uint64_t j = (uint64_t)i * i; j <= BASE_LIM; j += i) s[j] = 1;
            }
        }
        free(s);
    }

    const uint64_t S = 1u << 20;
    unsigned char *seg = (unsigned char *)malloc(S);
    long long sum = 0;
    uint64_t result = 0;
    for (uint64_t lo = 0; ; lo += S) {
        uint64_t hi = lo + S;
        if (hi > (uint64_t)BASE_LIM * BASE_LIM) break;
        memset(seg, 0, S);
        for (size_t i = 0; i < nbp; i++) {
            uint64_t p = bp[i];
            if (p * p >= hi) break;
            uint64_t st = p * p;
            if (st < lo) st = ((lo + p - 1) / p) * p;
            for (uint64_t j = st; j < hi; j += p) seg[j - lo] = 1;
        }
        for (uint64_t k = 0; k < S; k++) {
            uint64_t v = lo + k;
            if (v < 2 || seg[k]) continue;
            sum += A306500_chi(d, v);
            if (sum > 0) { result = v; goto done; }
        }
    }
done:
    free(seg);
    return (seq_t)result;
}
