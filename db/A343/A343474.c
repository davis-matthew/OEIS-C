/*
 * A343474  a(n) is the number of preference profiles for n men and n women,
 *          where all men prefer the same woman and all women prefer the same
 *          man.
 * https://oeis.org/A343474
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^2 * ((n-1)!)^(2*n)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Validated: partial, pass, 8 terms through n = 8 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"
#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;          // convert idx_t to seq_t
    seq_t fact = 1;              // (n-1)! will be stored here
    for (idx_t i = 2; i < n; ++i) {
        fact *= i;               // multiply by each integer
    }
    seq_t exp = (seq_t)(2 * n);  // exponent 2*n
    seq_t power = ipow(fact, exp); // compute ((n-1)!)^(2*n)

    seq_t result = power;       // multiply by n^2
    result *= N;
    result *= N;
    return result;
}
