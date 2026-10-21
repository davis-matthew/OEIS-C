/*
 * A297025  Number of iterations of A220096 required to reach 0 starting from
 *          n.
 * https://oeis.org/A297025
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0, a(1)=1;
   For n>1: a(n)=1+a(f(n)),
   where f(n)=n-1 if n is prime;
   otherwise f(n)=n / spf(n).   */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/_is_prime_u64.h"

static idx_t _smallest_prime_factor(idx_t n) {
    if (n % 2 == 0) return 2;
    if (n % 3 == 0) return 3;
    for (idx_t i = 5; i <= n / i; i += 6) {
        if (n % i == 0) return i;
        if (n % (i + 2) == 0) return i + 2;
    }
    return n; /* n is prime */
}

seq_t a(idx_t n) {
    seq_t steps = 0;
    while (true) {
        if (n == 0) break;
        if (n == 1) {
            steps++;
            break;
        }
        idx_t next;
        if (_is_prime_u64(n)) {
            next = n - 1;
        } else {
            idx_t spf = _smallest_prime_factor(n);
            next = n / spf;
        }
        steps++;
        n = next;
    }
    return steps;
}
