/*
 * A380870  a(n) = A381798(n) - A361373(n) - 1.
 * https://oeis.org/A380870
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |R(n) \ Q(n)| where Q(n) = {1} ∪ {p^k | p | n, 1 ≤ k, p^k
 *            ≤ n} and R(n) = ⋃_{p|n} { p^m (mod n) | m ≥ 0 } (0 is ignored).
 *            The function factorises n, builds the two sets, and counts
 *            residues that are not prime‑power numbers. Special case n = 1
 *            returns 0
 * Range:     u8 n <= 537, u16 n <= 9253, u32 n <= 9253, u64 n <= 9253,
 *            u128 n <= 9253, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle the trivial case */
    if (n <= 1) return 0;

    /* Allocate arrays for Q and R */
    bool *q = (bool *)calloc((size_t)(n + 1), sizeof(bool));
    bool *r = (bool *)calloc((size_t)n, sizeof(bool));
    if (!q || !r) {
        free(q); free(r);
        return 0;
    }
    q[1] = true;          /* 1 is always in Q */

    /* ---- Build Q: all p^k ≤ n for prime divisors p of n ---- */
    uint64_t tmp = n;
    for (uint64_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            /* p is a prime factor */
            uint64_t pow = p;
            while (pow <= n) {
                q[pow] = true;
                if (pow > n / p) break;   /* avoid overflow */
                pow *= p;
            }
            while (tmp % p == 0) tmp /= p;
        }
    }
    if (tmp > 1) { /* last prime factor */
        uint64_t p = tmp;
        uint64_t pow = p;
        while (pow <= n) {
            q[pow] = true;
            if (pow > n / p) break;
            pow *= p;
        }
    }

    /* ---- Build R: residues of prime powers modulo n (0 excluded) ---- */
    tmp = n;
    for (uint64_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            /* Process prime divisor p */
            bool *visited = (bool *)calloc((size_t)n, sizeof(bool));
            uint64_t res = 1 % n;
            while (1) {
                if (res != 0) r[res] = true;
                if (visited[res]) { free(visited); break; }
                visited[res] = true;
                res = (res * p) % n;
            }
            while (tmp % p == 0) tmp /= p;
        }
    }
    if (tmp > 1) { /* last prime factor */
        uint64_t p = tmp;
        bool *visited = (bool *)calloc((size_t)n, sizeof(bool));
        uint64_t res = 1 % n;
        while (1) {
            if (res != 0) r[res] = true;
            if (visited[res]) { free(visited); break; }
            visited[res] = true;
            res = (res * p) % n;
        }
    }

    /* ---- Count residues that are not in Q ---- */
    seq_t count = 0;
    for (uint64_t i = 1; i < n; ++i) {
        if (r[i] && !q[i]) ++count;
    }

    free(q);
    free(r);
    return count;
}
