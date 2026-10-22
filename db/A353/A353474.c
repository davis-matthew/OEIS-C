/*
 * A353474  a(n) = 1 if n is a number of the form p^2 * q, where p and q are
 *          primes with p < q, otherwise 0.
 * https://oeis.org/A353474
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n = p^2 * q with primes p < q, else 0
 * Range:     u8 n <= 9922, u16 n <= 9922, u32 n <= 9922, u64 n <= 9922,
 *            u128 n <= 9922, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n) {
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;
    factor_u64((uint64_t)n, primes, exps, &cnt);
    if (cnt != 2)
        return 0;

    uint64_t p, q;
    if (exps[0] == 2 && exps[1] == 1) {
        p = primes[0];
        q = primes[1];
    } else if (exps[1] == 2 && exps[0] == 1) {
        p = primes[1];
        q = primes[0];
    } else
        return 0;

    return (p < q) ? (seq_t)1 : (seq_t)0;
}
