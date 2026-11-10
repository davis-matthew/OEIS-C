/*
 * A264900  Integers k such that A122102(k) + 1 is prime.
 * https://oeis.org/A264900
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search: a(n) = n-th k such that Sum_{i=1..k} prime(i)^4 + 1 is
 *            prime (running sum of prime(i)^4, Miller-Rabin test on sum + 1)
 * Range:     u8 n <= 9, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Validated: partial, pass, 48 terms through n = 48 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

typedef unsigned __int128 A264900_u128;

static bool A264900_small_prime(uint64_t p) {
    if (p < 2) return false;
    if (p % 2 == 0) return p == 2;
    for (uint64_t d = 3; d * d <= p; d += 2)
        if (p % d == 0) return false;
    return true;
}

/* m < 2^127 so additions never overflow */
static A264900_u128 A264900_mulmod(A264900_u128 a, A264900_u128 b, A264900_u128 m) {
    if ((a >> 64) == 0 && (b >> 64) == 0 && (m >> 64) == 0)
        return (a * b) % m;
    A264900_u128 r = 0;
    a %= m;
    while (b) {
        if (b & 1) { r += a; if (r >= m) r -= m; }
        a += a; if (a >= m) a -= m;
        b >>= 1;
    }
    return r;
}

static A264900_u128 A264900_powmod(A264900_u128 b, A264900_u128 e, A264900_u128 m) {
    A264900_u128 r = 1 % m;
    b %= m;
    while (e) {
        if (e & 1) r = A264900_mulmod(r, b, m);
        e >>= 1;
        if (e) b = A264900_mulmod(b, b, m);
    }
    return r;
}

/* Deterministic Miller-Rabin for m < 3.3e24 (bases: primes up to 41) */
static bool A264900_is_prime(A264900_u128 m) {
    static const unsigned sp[] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,73,79,83,89,97};
    if (m < 2) return false;
    for (int i = 0; i < 25; i++) {
        if (m == sp[i]) return true;
        if (m % sp[i] == 0) return false;
    }
    if (m < 97 * 97) return true;
    A264900_u128 d = m - 1;
    int s = 0;
    while ((d & 1) == 0) { d >>= 1; s++; }
    for (int i = 0; i < 13; i++) {
        A264900_u128 x = A264900_powmod(sp[i], d, m);
        if (x == 1 || x == m - 1) continue;
        bool comp = true;
        for (int r = 1; r < s; r++) {
            x = A264900_mulmod(x, x, m);
            if (x == m - 1) { comp = false; break; }
        }
        if (comp) return false;
    }
    return true;
}

seq_t a(idx_t n) {
    A264900_u128 s = 0;
    idx_t count = 0;
    idx_t k = 0;
    uint64_t p = 1;
    while (1) {
        do { p++; } while (!A264900_small_prime(p));
        k++;
        A264900_u128 q = (A264900_u128)p;
        A264900_u128 q2 = q * q;
        s += q2 * q2;
        /* for k >= 2, s+1 is even unless s is even */
        if (k >= 2 && (s & 1) != 0) continue;
        if (A264900_is_prime(s + 1)) {
            count++;
            if (count == n) return (seq_t)k;
        }
    }
}
