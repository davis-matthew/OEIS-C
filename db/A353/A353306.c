/*
 * A353306  Numbers k such that 1 is in the transitive closure of the map x ->
 *          A353313(x) when starting iterating from x=k.
 * https://oeis.org/A353306
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k >= 1 such that iterating A353313 (x -> x/3 if 3|x
 *            else 5*(x/3)+3+x%3) from k reaches 1; found by forward-iteration
 *            search
 * Range:     u8 n <= 11, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Validated: partial, pass, 56 terms through n = 56 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

#define A353306_CAP 20000

static uint64_t A353306_terms[A353306_CAP];
static idx_t A353306_count = 0;
static uint64_t A353306_next = 1;

static bool A353306_reaches_one(uint64_t k)
{
    uint64_t x = k;
    int steps;
    for (steps = 0; steps < 20000; steps++) {
        if (x == 1) return true;
        if (x == 0) return false;
        if (x > 1000000000000000000ULL) return false;
        uint64_t q = x / 3, r = x % 3;
        x = (r == 0) ? q : 5 * q + 3 + r;
    }
    return false;
}

seq_t a(idx_t n)
{
    while (A353306_count < n && A353306_count < A353306_CAP) {
        if (A353306_reaches_one(A353306_next)) {
            A353306_terms[A353306_count++] = A353306_next;
        }
        A353306_next++;
    }
    if (n <= A353306_count) return (seq_t)A353306_terms[n - 1];

    /* beyond cache: continue scanning without storing */
    idx_t c = A353306_count;
    uint64_t k = A353306_next;
    while (1) {
        if (A353306_reaches_one(k)) {
            c++;
            if (c == n) return (seq_t)k;
        }
        k++;
    }
}
