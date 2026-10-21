/*
 * A243530  Prime numbers containing the string 14.
 * https://oeis.org/A243530
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = nth prime containing the decimal substring 14
 * Range:     u8 n <= 1, u16 n <= 237, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   contains14, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/contains14.h"

seq_t a(idx_t n) {
    idx_t count = 0;
    for (idx_t candidate = 1;; ++candidate) {
        if (!contains14(candidate)) continue;
        if (is_prime(candidate)) {
            ++count;
            if (count == n)
                return (seq_t)candidate;
        }
    }
}
