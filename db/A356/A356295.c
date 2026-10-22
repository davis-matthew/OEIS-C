/*
 * A356295  Numbers that are not the sum of a nonnegative cube and a prime.
 * https://oeis.org/A356295
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 47, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  
 * Sequence A356295: Numbers that are NOT representable as the sum of
 * a non‑negative cube and a prime.
 * The function enumerates natural numbers in increasing order and
 * returns the nth such number (offset 1).
 * The implementation is a simple brute‑force search with on‑the‑fly
 * primality testing using the provided is_prime_big routine.
 */
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n) {
    /* Convert the 64‑bit index to a native type for looping. */
    uint64_t target = (uint64_t)n;
    uint64_t count  = 0;
    uint64_t x      = 1;          /* Candidate value to test */

    while (count < target) {
        bool ok = true;           /* Assume x is a term until proven otherwise */

        /* Compute integer cube root: largest k such that k^3 <= x */
        uint64_t kmax = 0;
        while (((kmax + 1) * (kmax + 1) * (kmax + 1)) <= x) {
            ++kmax;
        }

        for (uint64_t k = 0; k <= kmax && ok; ++k) {
            uint64_t cube = k * k * k;
            seq_t diff    = (seq_t)x - (seq_t)cube;  /* x - k^3 */

            if (diff >= 2) {
                if (is_prime_big(diff)) {
                    ok = false;  /* x can be expressed as prime + cube */
                }
            }
        }

        if (ok) {
            ++count;
            if (count == target) {
                return (seq_t)x;
            }
        }
        ++x;  /* Move to next candidate */
    }
    /* The loop always returns; the following is only for satisfying
       compiler warnings about missing return. */
    return (seq_t)0;
}
