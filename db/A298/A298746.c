/*
 * A298746  Numbers n whose base-2 representation can be written as the
 *          concatenation of the base-2 representations of a list of prime
 *          numbers, allowing leading zeros.
 * https://oeis.org/A298746
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A298746: numbers whose binary representation can be written as the concatenation
   of the binary representations of primes (leading zeros in parts allowed). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Simple deterministic primality test for 64‑bit integers. */
static bool isPrime(uint64_t x)
{
    if (x < 2) return false;
    if (x % 2 == 0) return x == 2;
    for (uint64_t d = 3; d <= x / d; d += 2)
        if (x % d == 0) return false;
    return true;
}

/* Check whether the binary representation of v can be partitioned into prime
   values (allowing leading zeros in each part). */
static bool is_concat_prime(uint64_t v)
{
    if (v == 0) return false;          /* 0 is not representable */

    /* Find bit length. */
    int len = 0;
    uint64_t t = v;
    while (t) { ++len; t >>= 1; }

    int bits[64];
    for (int i = 0; i < len; ++i)
        bits[i] = (v >> (len - 1 - i)) & 1u;

    bool f[65];
    f[len] = true;                     /* empty suffix is valid */

    for (int i = len - 1; i >= 0; --i) {
        f[i] = false;
        uint64_t val = 0;
        for (int j = i; j < len; ++j) {
            val = (val << 1) | (uint64_t)bits[j];
            if (val > 1 && isPrime(val) && f[j + 1]) {
                f[i] = true;
                break;
            }
        }
    }
    return f[0];
}

/* Static cache of the sequence values. */
static seq_t *seq_cache      = NULL;  /* array of seq_t */
static size_t  seq_cap       = 0;      /* capacity of seq_cache */
static size_t  seq_len       = 0;      /* number of elements stored */
static uint64_t last_checked = 1;     /* integer that has been tested up to */

/* External function producing the nth term of the sequence (1‑based). */
seq_t a(idx_t n)
{
    /* Ensure we have computed at least 'n' terms. */
    size_t target = (size_t)n;  /* n >= 1 */
    while (seq_len < target) {
        uint64_t candidate = last_checked + 1;
        while (true) {
            if (is_concat_prime(candidate)) {
                /* Expand cache if necessary. */
                if (seq_len == seq_cap) {
                    size_t new_cap = seq_cap ? seq_cap << 1 : 16;
                    /* Cast realloc result to seq_t* to satisfy C++ compilers. */
                    seq_cache = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
                    seq_cap = new_cap;
                }
                seq_cache[seq_len++] = (seq_t)candidate;
                break;
            }
            ++candidate;
        }
        last_checked = candidate;
    }
    return seq_cache[n - 1];
}
