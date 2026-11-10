/*
 * A340618  Numbers k such that A316650(k) is prime.
 * https://oeis.org/A340618
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   search: a(n) = n-th k >= 1 such that A316650(k) =
 *            concat(floor(k/ds(k)), k mod ds(k)) is prime
 * Range:     u8 n <= 48, u16 n <= 7138, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 80 terms through n = 9708 (2026-11-10)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_u64.h"

/* A316650 computed in uint64_t so the primality test sees the exact value
   regardless of the width of seq_t. */
static uint64_t A316650_a_u64(uint64_t n)
{
    if (n < 1)
        return 0;
    uint64_t sum = 0, tmp = n;
    while (tmp) {
        sum += tmp % 10;
        tmp /= 10;
    }
    uint64_t q = n / sum;
    uint64_t r = n % sum;
    uint64_t pow10 = 10, t = r;
    while (t >= 10) {
        t /= 10;
        pow10 *= 10;
    }
    return q * pow10 + r;
}

static idx_t *A340618_cache = 0;
static size_t A340618_cnt = 0;
static size_t A340618_cap = 0;
static idx_t A340618_next = 1;

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;
    while (A340618_cnt < (size_t)n) {
        idx_t k = A340618_next++;
        if (is_prime_u64(A316650_a_u64((uint64_t)k))) {
            if (A340618_cnt == A340618_cap) {
                size_t nc = A340618_cap ? A340618_cap * 2 : 1024;
                idx_t *p = (idx_t *)realloc(A340618_cache, nc * sizeof(idx_t));
                if (!p)
                    return 0;
                A340618_cache = p;
                A340618_cap = nc;
            }
            A340618_cache[A340618_cnt++] = k;
        }
    }
    return (seq_t)A340618_cache[n - 1];
}
