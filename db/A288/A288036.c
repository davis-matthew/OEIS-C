/*
 * A288036  Numbers x such that the trajectory of x under the map x ->
 *          A090086(x) does not enter the cycle {14, 15}.
 * https://oeis.org/A288036
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search: a(n) = n-th x >= 1 whose trajectory under x ->
 *            A090086(x) never reaches the cycle {14, 15} (cycle detection
 *            with Brent's method)
 * Range:     u8 n <= 5, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Validated: partial, pass, 46 terms through n = 46 (2026-11-08)
 * Helpers:   is_prime_u64, powmod_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime_u64.h"
#include "../utils/powmod_u64.h"

/* A090086: smallest composite m with x^(m-1) == 1 (mod m) */
static uint64_t A288036_f(uint64_t x)
{
    for (uint64_t m = 4; ; ++m) {
        if (is_prime_u64(m)) continue;
        if (powmod_u64(x % m, m - 1, m) == 1) return m;
    }
}

static bool A288036_enters(uint64_t x)
{
    if (x == 14 || x == 15) return true;
    uint64_t tort = x, hare = A288036_f(x);
    uint64_t power = 1, lam = 1;
    while (true) {
        if (hare == 14 || hare == 15) return true;
        if (tort == hare) return false;
        if (power == lam) { tort = hare; power *= 2; lam = 0; }
        hare = A288036_f(hare);
        lam++;
    }
}

static uint64_t *A288036_terms = NULL;
static size_t A288036_cnt = 0, A288036_cap = 0;
static uint64_t A288036_next = 1;

seq_t a(idx_t n)
{
    while (A288036_cnt < (size_t)n) {
        uint64_t x = A288036_next++;
        if (!A288036_enters(x)) {
            if (A288036_cnt == A288036_cap) {
                A288036_cap = A288036_cap ? A288036_cap * 2 : 64;
                A288036_terms = (uint64_t *)realloc(A288036_terms, A288036_cap * sizeof(uint64_t));
            }
            A288036_terms[A288036_cnt++] = x;
        }
    }
    return (seq_t)A288036_terms[(size_t)n - 1];
}
