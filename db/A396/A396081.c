/*
 * A396081  Primes p such that A381466(2p) < 2p.
 * https://oeis.org/A396081
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th prime p with A381466(2p) = p, where A381466(0)=4,
 *            A381466(k) = A381466(k-1)+k if gcd(A381466(k-1),k)=1, else
 *            k/gcd; found by running the recurrence
 * Range:     u8 n <= 16, u16 n <= 1763, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 69 terms through n = 9197 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static uint64_t *A396081_terms = NULL;
static size_t A396081_cnt = 0, A396081_cap = 0;
static uint64_t A396081_i = 0;      /* last processed index */
static uint64_t A396081_prev = 4;   /* A381466(A396081_i) */

static uint64_t A396081_gcd(uint64_t a, uint64_t b)
{
    while (b) { uint64_t t = a % b; a = b; b = t; }
    return a;
}

static bool A396081_isprime(uint64_t p)
{
    if (p < 2) return false;
    if (p % 2 == 0) return p == 2;
    for (uint64_t d = 3; d * d <= p; d += 2)
        if (p % d == 0) return false;
    return true;
}

seq_t a(idx_t n)
{
    while (A396081_cnt < (size_t)n) {
        uint64_t i = ++A396081_i;
        uint64_t g = A396081_gcd(A396081_prev, i);
        uint64_t v = (g == 1) ? A396081_prev + i : i / g;
        A396081_prev = v;
        if ((i & 1) == 0 && v == i / 2 && A396081_isprime(v)) {
            if (A396081_cnt == A396081_cap) {
                size_t nc = A396081_cap ? A396081_cap * 2 : 64;
                uint64_t *t = (uint64_t *)realloc(A396081_terms, nc * sizeof(uint64_t));
                if (!t) abort();
                A396081_terms = t;
                A396081_cap = nc;
            }
            A396081_terms[A396081_cnt++] = v;
        }
    }
    return (seq_t)A396081_terms[n - 1];
}
