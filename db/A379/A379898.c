/*
 * A379898  Integers k equal to the sum over A003415(t) mod t, for some steps,
 *          starting with t = k and then using the result for the next
 *          calculation.
 * https://oeis.org/A379898
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th integer k>=2 such that k = t1+t2+...+tj for some j,
 *            where t0=k, t_{i+1} = A003415(t_i) mod t_i; found by incremental
 *            search
 * Range:     u8 n <= 5, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Validated: partial, pass, 49 terms through n = 49 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

static seq_t A003415_a(seq_t n) {
    if (n == 0 || n == 1)
        return 0;
    seq_t result = 0;
    seq_t remaining = n;
    for (seq_t p = 2; p * p <= remaining; p++) {
        int exponent = 0;
        while (remaining % p == 0) {
            exponent++;
            remaining /= p;
        }
        if (exponent > 0) {
            result += (n / p) * (seq_t)exponent;
        }
    }
    if (remaining > 1)
        result += n / remaining;
    return result;
}

static bool A379898_check(seq_t k) {
    seq_t sum = 0;
    seq_t t = k;
    while (t >= 2) {
        t = A003415_a(t) % t;
        sum += t;
        if (sum >= k) break;
    }
    return sum == k;
}

static seq_t *A379898_terms = 0;
static idx_t A379898_count = 0;
static idx_t A379898_cap = 0;
static seq_t A379898_next = 2;

seq_t a(idx_t n) {
    while (A379898_count < n) {
        if (A379898_check(A379898_next)) {
            if (A379898_count >= A379898_cap) {
                idx_t ncap = A379898_cap ? A379898_cap * 2 : 64;
                A379898_terms = mem_resize(seq_t, A379898_terms, ncap);
                A379898_cap = ncap;
            }
            A379898_terms[A379898_count++] = A379898_next;
        }
        A379898_next++;
    }
    return A379898_terms[n - 1];
}
