/*
 * A367577  a(n) is the first k such that A367566(k) = n, or -1 if no such k
 *          exists.
 * https://oeis.org/A367577
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   search. a(n) = -1 if n is nonsquarefree or the conditions it
 *            imposes contradict each other (p | n with p >= 5 needs (p-1)/2 |
 *            k and k == +-1 (mod p); 2 | A367566(k) iff k is odd; 3 |
 *            A367566(k) iff k > 1 and 3 does not divide k). Otherwise the
 *            code steps k through multiples of lcm((p-1)/2, ...) a...
 * Range:     i8 none, i16 n <= 61, i32 n <= 61, i64 n <= 61, i128 n <= 61,
 *            big any n
 * Validated: partial, pass, 61 terms through n = 61 (2026-11-10)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime_u64.h"

static uint64_t A367577_gcd(uint64_t a, uint64_t b)
{
    while (b) { uint64_t t = a % b; a = b; b = t; }
    return a;
}

/* Does some divisor d >= 2 of k give a prime q = 2d+1 <= k+1 with k == +-1 (mod q) and q not dividing n? */
static bool A367577_bad_div(uint64_t k, idx_t n, const uint64_t *pr, const int *ex,
                            int cnt, int i, uint64_t d)
{
    if (i == cnt) {
        if (d < 2) return false;
        uint64_t q = 2 * d + 1;
        if (q > k + 1) return false;
        uint64_t r = k % q;
        if (r != 1 && r != q - 1) return false;
        if (!is_prime_u64(q)) return false;
        return (n % q) != 0;
    }
    uint64_t dd = d;
    for (int e = 0; e <= ex[i]; e++) {
        if (A367577_bad_div(k, n, pr, ex, cnt, i + 1, dd)) return true;
        dd *= pr[i];
    }
    return false;
}

static bool A367577_check(uint64_t k, idx_t n, const uint64_t *P, int np)
{
    bool inc2 = (k & 1) != 0;
    if (inc2 != (n % 2 == 0)) return false;
    bool inc3 = (k > 1 && k % 3 != 0);
    if (inc3 != (n % 3 == 0)) return false;
    for (int i = 0; i < np; i++) {
        uint64_t p = P[i];
        if (p > k + 1) return false;
        uint64_t r = k % p;
        if (r != 1 && r != p - 1) return false;
        if (k % ((p - 1) / 2) != 0) return false;
    }
    /* factor k */
    uint64_t pr[64];
    int ex[64];
    int cnt = 0;
    uint64_t m = k;
    for (uint64_t q = 2; q * q <= m; q += (q == 2 ? 1 : 2)) {
        if (m % q == 0) {
            int e = 0;
            while (m % q == 0) { m /= q; e++; }
            pr[cnt] = q; ex[cnt] = e; cnt++;
        }
    }
    if (m > 1) { pr[cnt] = m; ex[cnt] = 1; cnt++; }
    return !A367577_bad_div(k, n, pr, ex, cnt, 0, 1);
}

seq_t a(idx_t n)
{
    uint64_t P[64];
    int np = 0;
    {
        idx_t m = n;
        for (idx_t q = 2; q * q <= m; q++) {
            if (m % q == 0) {
                m /= q;
                if (m % q == 0) return (seq_t)-1;
                if (q >= 5) P[np++] = q;
            }
        }
        if (m > 1 && m >= 5) P[np++] = m;
    }
    if (n == 2) return (seq_t)1;   /* A367566(1) = 2 */

    /* consistency of the forced congruences */
    for (int i = 0; i < np; i++) {
        uint64_t h = (P[i] - 1) / 2;
        if (h % 2 == 0 && n % 2 == 0) return (seq_t)-1;
        if (h % 3 == 0 && n % 3 == 0) return (seq_t)-1;
        for (int j = 0; j < np; j++)
            if (j != i && h % P[j] == 0) return (seq_t)-1;
    }

    uint64_t L = 1;
    for (int i = 0; i < np; i++) {
        uint64_t h = (P[i] - 1) / 2;
        L = L / A367577_gcd(L, h) * h;
    }
    if (n % 2 != 0) L = L / A367577_gcd(L, 2) * 2;
    if (n % 3 != 0) L = L / A367577_gcd(L, 3) * 3;

    for (uint64_t t = 1; t <= (uint64_t)400000000; t++) {
        uint64_t k = L * t;
        if (A367577_check(k, n, P, np)) return (seq_t)k;
    }
    return (seq_t)-1;
}
