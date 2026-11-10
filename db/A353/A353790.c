/*
 * A353790  a(n) = A353749(sigma(A003961(n))), where A353749(k) = phi(k) *
 *          A064989(k), and A064989 shifts the prime factorization one step
 *          towards lower primes, while A003961 shifts the factorization one
 *          step towards higher primes.
 * https://oeis.org/A353790
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = phi(k) * A064989(k) with k = A003973(n) =
 *            sigma(A003961(n)); computed multiplicatively from the
 *            factorization of k
 * Range:     u8 n <= 8, u16 n <= 35, u32 n <= 1023, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 81 terms through n = 8516 (2026-11-10)
 * Helpers:   mem, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/next_prime.h"
#include "../utils/mem.h"

static bool a353790_isprime(seq_t x) {
    if (x < 2) return false;
    for (seq_t d = 2; d * d <= x; d++)
        if (x % d == 0) return false;
    return true;
}

static seq_t a353790_prevprime(seq_t p) {
    seq_t x = p - 1;
    while (x > 2 && !a353790_isprime(x)) x--;
    return x;
}

static void a353790_add(seq_t *pr, int *ex, int *cnt, seq_t p, int e) {
    for (int i = 0; i < *cnt; i++) {
        if (pr[i] == p) { ex[i] += e; return; }
    }
    pr[*cnt] = p;
    ex[*cnt] = e;
    (*cnt)++;
}

seq_t a(idx_t n) {
    seq_t *pr = mem_new(seq_t, 1024);
    int *ex = mem_new(int, 1024);
    int cnt = 0;

    idx_t m = n;
    for (idx_t p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            int e = 0;
            while (m % p == 0) { m /= p; e++; }
            seq_t q = (seq_t)next_prime((uint64_t)p);
            seq_t s = 1, pw = 1;
            for (int i = 0; i < e; i++) { pw *= q; s += pw; }
            for (seq_t d = 2; d * d <= s; d++) {
                if (s % d == 0) {
                    int c = 0;
                    while (s % d == 0) { s /= d; c++; }
                    a353790_add(pr, ex, &cnt, d, c);
                }
            }
            if (s > 1) a353790_add(pr, ex, &cnt, s, 1);
        }
    }
    if (m > 1) {
        seq_t q = (seq_t)next_prime((uint64_t)m);
        seq_t s = 1 + q;
        for (seq_t d = 2; d * d <= s; d++) {
            if (s % d == 0) {
                int c = 0;
                while (s % d == 0) { s /= d; c++; }
                a353790_add(pr, ex, &cnt, d, c);
            }
        }
        if (s > 1) a353790_add(pr, ex, &cnt, s, 1);
    }

    seq_t result = 1;
    for (int i = 0; i < cnt; i++) {
        seq_t p = pr[i];
        int e = ex[i];
        if (p == 2) {
            for (int j = 0; j < e - 1; j++) result *= 2;
        } else {
            seq_t pp = a353790_prevprime(p);
            result *= (p - 1);
            for (int j = 0; j < e - 1; j++) result *= p;
            for (int j = 0; j < e; j++) result *= pp;
        }
    }
    mem_free(pr);
    mem_free(ex);
    return result;
}
