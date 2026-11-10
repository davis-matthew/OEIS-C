/*
 * A270385  Numbers n such that n^2 is a term of A007692 while n is not.
 * https://oeis.org/A270385
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k (increasing) such that k^2 is a sum of two nonzero
 *            squares in >= 2 ways while k is not (search)
 * Range:     u8 n <= 9, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Validated: partial, pass, 54 terms through n = 54 (2026-11-07)
 * Helpers:   isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/isqrt_u64.h"

static int A270385_reps(uint64_t k) {
    int count = 0;
    for (uint64_t x = 1; 2 * x * x <= k; x++) {
        uint64_t r = k - x * x;
        uint64_t y = isqrt_u64(r);
        if (y >= x && y * y == r) {
            count++;
            if (count >= 2) return count;
        }
    }
    return count;
}

seq_t a(idx_t n) {
    static uint64_t *terms = NULL;
    static idx_t computed = 0;
    static idx_t capacity = 0;
    static uint64_t next_k = 1;

    while (computed < n) {
        uint64_t k = next_k++;
        if (A270385_reps(k) < 2 && A270385_reps(k * k) >= 2) {
            if (computed >= capacity) {
                capacity = capacity ? capacity * 2 : 128;
                terms = (uint64_t *)realloc(terms, (size_t)capacity * sizeof(uint64_t));
            }
            terms[computed++] = k;
        }
    }
    return (seq_t)terms[n - 1];
}
