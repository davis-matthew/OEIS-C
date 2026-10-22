/*
 * A354819  a(n) = 1 if n is a nonprime squarefree number, otherwise 0.
 * https://oeis.org/A354819
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n is a nonprime squarefree number; otherwise 0
 * Range:     u8 n <= 1385, u16 n <= 6484, u32 n <= 6484, u64 n <= 6484,
 *            u128 n <= 6484, big any n
 * Helpers:   is_prime, is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/is_squarefree.h"

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;
    return (is_squarefree((seq_t)n) && !is_prime((idx_t)n)) ? (seq_t)1 : (seq_t)0;
}
