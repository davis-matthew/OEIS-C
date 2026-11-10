/*
 * A309627  Starting numbers k such that the trajectory of k under the map x
 *          -> A133058(x) joins A133058.
 * https://oeis.org/A309627
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search over k = 1, 2, 3, ...: let f(0) = k and, for n >= 1, *
 *            f(n) = f(n-1) + n + 1 if gcd(f(n-1), n) = 1, else f(n) =
 *            f(n-1)/gcd(f(n-1), n). * k is a term if f(n) = A133058(n) for
 *            some n >= 1 before A133058 enters its * final 4-cycle (1, n+2,
 *            2n+2, 2). Every trajectory that enters that cycle * s...
 * Range:     u8 n <= 70, u16 n <= 306, u32 n <= 306, u64 n <= 306,
 *            u128 n <= 306, big any n
 * Validated: standard, pass, 77 terms through n = 278 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint64_t A309627_gcd(uint64_t a, uint64_t b)
{
    while (b != 0) {
        uint64_t t = b;
        b = a % b;
        a = t;
    }
    return a;
}

static uint64_t A309627_step(uint64_t x, uint64_t i)
{
    uint64_t g = A309627_gcd(x, i);
    if (g == 1)
        return x + i + 1;
    return x / g;
}

/* is (index n, value x) a state of the closed cycle 1, n+2, 2n+2, 2 ? */
static bool A309627_trapped(uint64_t n, uint64_t x)
{
    switch (n % 4) {
    case 2: return x == 1;
    case 3: return x == n + 2;
    case 0: return x == 2 * n + 2;
    default: return x == 2;
    }
}

static uint64_t *A309627_ref = 0;   /* A133058(0..n0-1) */
static uint64_t A309627_n0 = 0;     /* first index where A133058 is trapped */
static idx_t *A309627_terms = 0;
static size_t A309627_cnt = 0, A309627_cap = 0;
static idx_t A309627_next = 1;

static void A309627_init(void)
{
    size_t cap = 1024;
    A309627_ref = (uint64_t *)malloc(cap * sizeof(uint64_t));
    A309627_ref[0] = 1;
    A309627_ref[1] = 1;
    uint64_t i = 1;
    for (;;) {
        uint64_t nx = A309627_step(A309627_ref[i], i + 1);
        ++i;
        if (A309627_trapped(i, nx)) {
            A309627_n0 = i;
            break;
        }
        if (i >= cap) {
            cap *= 2;
            A309627_ref = (uint64_t *)realloc(A309627_ref, cap * sizeof(uint64_t));
        }
        A309627_ref[i] = nx;
    }
}

static bool A309627_joins(uint64_t k)
{
    uint64_t x = k;
    for (uint64_t i = 1; i < A309627_n0; ++i) {
        x = A309627_step(x, i);
        if (x == A309627_ref[i])
            return true;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (!A309627_ref)
        A309627_init();
    while (A309627_cnt < (size_t)n) {
        idx_t k = A309627_next++;
        if (A309627_joins((uint64_t)k)) {
            if (A309627_cnt == A309627_cap) {
                A309627_cap = A309627_cap ? A309627_cap * 2 : 64;
                A309627_terms = (idx_t *)realloc(A309627_terms, A309627_cap * sizeof(idx_t));
            }
            A309627_terms[A309627_cnt++] = k;
        }
    }
    return (seq_t)A309627_terms[n - 1];
}
