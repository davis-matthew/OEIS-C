/*
 * A333634  Numbers with an even number of non-unitary prime divisors.
 * https://oeis.org/A333634
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 168, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   ensure_capacity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A333634: numbers with an even number of distinct primes occurring to power ≥2.
    The sequence is computed by testing each integer in succession and recording
    those for which the count of primes with exponent ≥2 is even (including 0). */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/ensure_capacity.h"

static seq_t *seq = NULL;      /* stored sequence values */
static size_t seq_len = 0;     /* current number of values stored */
static size_t seq_cap = 0;     /* current capacity of the array */
static uint64_t next_check = 1;/* next integer to test */

/*  Return true if x has an even number of distinct prime divisors with exponent ≥ 2. */
static inline bool has_property(uint64_t x)
{
    if (x <= 1) return true;        /* 1 has 0 such primes, which is even */

    uint64_t n = x;
    bool parity = false;            /* true -> odd count, false -> even count */

    /* handle factor 2 */
    if (n % 2 == 0) {
        int exp = 0;
        while (n % 2 == 0) {
            n /= 2;
            ++exp;
        }
        if (exp >= 2) parity = !parity;
    }

    /* trial division by odd numbers */
    for (uint64_t p = 3; p * p <= n; p += 2) {
        if (n % p == 0) {
            int exp = 0;
            while (n % p == 0) {
                n /= p;
                ++exp;
            }
            if (exp >= 2) parity = !parity;
        }
    }

    /* remaining n > 1 is prime with exponent 1 – it does not affect parity */
    return !parity;   /* true if count is even */
}

/* append val to the sequence array, resizing if necessary */
static inline void append_seq(uint64_t val)
{
    ensure_capacity(seq_len + 1, &seq_cap, (void **)&seq, sizeof(seq_t));
    seq[seq_len++] = (seq_t)val;
}

/*  External linkage required by the build system */
seq_t a(idx_t n)
{
    /* Non‑positive indices are undefined; return 0 (not used). */
    if (n <= 0) return (seq_t)0;

    /* Generate sequence values until we have at least n elements. */
    while (seq_len < (size_t)n) {
        if (has_property(next_check))
            append_seq(next_check);
        ++next_check;
    }

    return seq[(size_t)n - 1];
}
