/*
 * A385375  Numbers k that can't be partitioned into tau(k) distinct parts.
 * https://oeis.org/A385375
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 14, u32 n <= 14, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Helpers:   my_divisor_count
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Implements the OEIS Definition: a(k) is the k‑th number such that
 * k < tau(k)*(tau(k)+1)/2, where tau(k) is the number of positive
 * divisors of k.  The sequence is finite (14 terms), so the function
 * returns 0 for indices beyond the last term.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/my_divisor_count.h"

/* Main sequence routine */
seq_t a(idx_t n)
{
    static bool initialized = false;
    static seq_t terms[14];
    static idx_t term_count = 0;

    if (!initialized) {
        for (idx_t k = 1; term_count < 14; ++k) {
            uint64_t tau = my_divisor_count((uint64_t)k);
            seq_t lower = ((seq_t)tau) * ((seq_t)tau + 1) / 2;

            if ((seq_t)k < lower) {
                terms[term_count++] = (seq_t)k;
            }
        }
        initialized = true;
    }

    if (n == 0 || n > term_count) {
        return (seq_t)0;
    }
    return terms[n - 1];
}
