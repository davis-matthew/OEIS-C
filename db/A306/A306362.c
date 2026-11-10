/*
 * A306362  Prime numbers in A317298.
 * https://oeis.org/A306362
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 3; for n >= 2, a(n) = the (n-1)th prime of the form 8*m^2
 *            + 2*m + 1 with m >= 1 (search)
 * Range:     u8 n <= 6, u16 n <= 39, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Validated: partial, pass, 42 terms through n = 42 (2026-11-08)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

seq_t a(idx_t n)
{
    static seq_t *primes = NULL;
    static size_t primes_cap = 0;
    static size_t primes_len = 0;
    static uint64_t next_m = 1;

    if (n <= 1) return (seq_t)3;

    size_t need = (size_t)(n - 1);
    while (primes_len < need) {
        uint64_t m = next_m++;
        uint64_t p = 8ULL * m * m + 2ULL * m + 1ULL;
        if (is_prime_u64(p)) {
            if (primes_len == primes_cap) {
                size_t nc = primes_cap ? primes_cap * 2 : 16;
                seq_t *tmp = mem_resize(seq_t, primes, nc);
                if (!tmp) return (seq_t)0;
                primes = tmp;
                primes_cap = nc;
            }
            primes[primes_len++] = (seq_t)p;
        }
    }
    return primes[need - 1];
}
