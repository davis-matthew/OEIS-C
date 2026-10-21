/*
 * A278799  Prime numbers that can be written as concatenation of two
 *          nonprimes in decimal representation.
 * https://oeis.org/A278799
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th prime that can be written as the concatenation
 *            of two non‑prime decimal numbers
 * Range:     u8 n <= 21, u16 n <= 5269, u32 n <= 7135, u64 n <= 7135,
 *            u128 n <= 7135, big any n
 * Helpers:   digit_count, is_prime_small, next_prime, seq_to_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function performs a simple incremental search for primes and tests each splice point. */
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/next_prime.h"
#include "../utils/digit_count.h"
#include "../utils/seq_to_str.h"
#include "../utils/is_prime_small.h"

static inline bool qualifies(uint64_t p) {
    /* Must have at least two digits. */
    if (p < 10ULL) return false;

    size_t len = digit_count(p);
    char buf[32];          /* 64‑bit numbers have <= 20 decimal digits. */
    size_t used;
    seq_to_str((seq_t)p, buf, &used);
    /* Use `used` to double‑check; it should equal `len`. */
    (void)len;            /* suppress unused warning if digit_count is unused. */

    /* Test every split between digits. */
    for (size_t split = 1; split < used; ++split) {
        uint64_t left = 0, right = 0;
        for (size_t i = 0; i < split; ++i)
            left = left * 10 + (buf[i] - '0');
        for (size_t i = split; i < used; ++i)
            right = right * 10 + (buf[i] - '0');

        if (!is_prime_small(left) && !is_prime_small(right))
            return true;
    }
    return false;
}

seq_t a(idx_t n) {
    if (n == 0)     /* undefined for n==0, but return 0 to keep type safety */
        return (seq_t)0;

    uint64_t count = 0;
    uint64_t current = 1;   /* start searching after 1 */

    for (;;) {
        current = next_prime(current + 1);
        if (qualifies(current)) {
            ++count;
            if (count == n)
                return (seq_t)current;
        }
    }
}
