/*
 * A375277  a(n) = n! (mod nextprime(n)).
 * https://oeis.org/A375277
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n! (mod nextprime(n))
 * Range:     u8 n <= 254, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/*  Simple deterministic primality test for n ≤ 2^64
    Runs in O(√n) – sufficient for the range n ≤ 10 000. */
static bool is_prime_uint64(uint64_t x) {
    if (x < 2)          return false;
    if (x == 2 || x == 3) return true;
    if ((x & 1) == 0)   return false;
    for (uint64_t d = 3; d <= x / d; d += 2)
        if (x % d == 0) return false;
    return true;
}

/* Smallest prime strictly greater than n */
static uint64_t next_prime_after(uint64_t n) {
    uint64_t p = n + 1;
    while (!is_prime_uint64(p)) ++p;
    return p;
}

seq_t a(idx_t n) {
    uint64_t prime = next_prime_after(n);
    seq_t   res    = 1;

    /* Compute n! modulo prime.  Use uint64_t for the intermediate
       product to avoid overflow in seq_t. */
    for (idx_t i = 2; i <= n; ++i) {
        uint64_t prod = (uint64_t)res * (uint64_t)i;
        prod %= prime;
        res = (seq_t)prod;
    }
    return res;
}
