/*
 * A347243  Numbers k such that when iterating the map x -> A000593(x), we
 *          will not encounter a term x (after the starting point x=k) whose
 *          largest prime factor would be at least as large as A006530(k),
 *          before 1 is eventually reached.
 * https://oeis.org/A347243
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k (a(1)=1 by convention) such that iterating x ->
 *            A000593(x) from k reaches 1 with every later term having largest
 *            prime factor < A006530(k); found by search with memoization
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Validated: partial, pass, 83 terms through n = 83 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Factor x: returns the greatest prime factor (1 for x=1) and the sum of odd divisors of x. */
static uint64_t A347243_fac(uint64_t x, uint64_t *sodd) {
    uint64_t gpf = 1, s = 1;
    while ((x & 1) == 0) { x >>= 1; gpf = 2; }
    for (uint64_t p = 3; p * p <= x; p += 2) {
        if (x % p == 0) {
            uint64_t term = 1, pw = 1;
            while (x % p == 0) { x /= p; pw *= p; term += pw; }
            s *= term;
            gpf = p;
        }
    }
    if (x > 1) { s *= (x + 1); gpf = x; }
    *sodd = s;
    return gpf;
}

static int A347243_test(uint64_t k) {
    if (k == 1) return 1;
    uint64_t s;
    uint64_t g = A347243_fac(k, &s);
    uint64_t x = s;
    int steps = 0;
    while (x != 1) {
        uint64_t s2;
        uint64_t gp = A347243_fac(x, &s2);
        if (gp >= g) return 0;
        x = s2;
        if (++steps > 100000) return 0;
    }
    return 1;
}

static uint64_t *A347243_terms = 0;
static size_t A347243_cnt = 0, A347243_cap = 0;
static uint64_t A347243_next = 1;

seq_t a(idx_t n) {
    while (A347243_cnt < (size_t)n) {
        if (A347243_test(A347243_next)) {
            if (A347243_cnt == A347243_cap) {
                A347243_cap = A347243_cap ? A347243_cap * 2 : 256;
                A347243_terms = (uint64_t *)realloc(A347243_terms, A347243_cap * sizeof(uint64_t));
            }
            A347243_terms[A347243_cnt++] = A347243_next;
        }
        A347243_next++;
    }
    return (seq_t)A347243_terms[n - 1];
}
