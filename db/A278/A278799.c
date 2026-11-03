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
 * Validated: standard, pass, 74 terms through n = 7107 (2026-11-01)
 * Helpers:   is_prime_seq, next_prime, num_digits0
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function performs a simple incremental search for primes and tests each splice point. */
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>
#include "../utils/is_prime_seq.h"
#include "../utils/next_prime.h"
#include "../utils/num_digits0.h"

/* convert a seq_t value to a decimal string */
static void seq_to_str(seq_t v, char *buf, size_t *len)
{
    if (v == 0) {
        buf[0] = '0';
        *len = 1;
        return;
    }
    char tmp[64];
    int pos = 0;
    seq_t ten = (seq_t)10;
    while (v != 0) {
        tmp[pos++] = '0' + (int)(v % ten);
        v /= ten;
    }
    for (int i = 0; i < pos; ++i)
        buf[i] = tmp[pos - 1 - i];
    *len = pos;
}

static inline bool qualifies(uint64_t p) {
    /* Must have at least two digits. */
    if (p < 10ULL) return false;

    size_t len = num_digits0(p);
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

        if (!is_prime_seq(left) && !is_prime_seq(right))
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
