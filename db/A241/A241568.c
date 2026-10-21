/*
 * A241568  a(n) = |{0 < k < prime(n)/2: k is not only a quadratic nonresidue
 *          modulo prime(n) but also a Fibonacci number}|.
 * https://oeis.org/A241568
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of Fibonacci k with 0 < k < prime(n)/2 that are
 *            quadratic nonresidues modulo prime(n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, jacobi_symbol
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/jacobi_symbol.h"

seq_t a(idx_t n) {
    uint32_t p = get_prime(n);
    uint32_t limit = (p - 1) / 2;
    uint32_t count = 0;

    if (limit >= 1) {
        if (jacobi_symbol(1ULL, p) == -1)
            count++;
    }

    uint64_t a1 = 1;
    uint64_t b = 2;
    while (b <= limit) {
        if (jacobi_symbol(b, p) == -1)
            count++;
        uint64_t next = a1 + b;
        a1 = b;
        b = next;
    }

    return (seq_t)count;
}
