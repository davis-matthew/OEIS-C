/*
 * A353137  Primes whose gaps to both neighbor primes are powers of two.
 * https://oeis.org/A353137
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 783, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_power_of_two, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: iterate primes sequentially and test whether
   both gaps to neighboring primes are powers of two. */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/next_prime.h"
#include "../utils/is_power_of_two.h"

seq_t a(idx_t n) {
    /* Start with first two primes: 2 and 3. */
    uint64_t prev = 2;
    uint64_t curr = 3;
    idx_t count = 0;

    while (1) {
        uint64_t next = next_prime(curr + 1);

        /* curr is a term if both gaps are powers of two. */
        if (is_power_of_two((seq_t)(curr - prev)) &&
            is_power_of_two((seq_t)(next - curr))) {
            count++;
            if (count == n)
                return (seq_t)curr;
        }

        /* move window forward */
        prev = curr;
        curr = next;
    }
}
