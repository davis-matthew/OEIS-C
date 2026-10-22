/*
 * A317295  Numbers with a composite number of 1's in their binary expansion.
 * https://oeis.org/A317295
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 99, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers with a composite number of 1's in binary expansion
   (OEIS A317295).  The function returns the nth such integer with
   offset 1, i.e. a(1) = 15, a(2) = 23, ... */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Small primality table for numbers 0..64.
       true if the index is a prime, false otherwise. */
    static const bool is_prime[65] = {
        /*  0 */ false,
        /*  1 */ false,
        /*  2 */ true,
        /*  3 */ true,
        /*  4 */ false,
        /*  5 */ true,
        /*  6 */ false,
        /*  7 */ true,
        /*  8 */ false,
        /*  9 */ false,
        /* 10 */ false,
        /* 11 */ true,
        /* 12 */ false,
        /* 13 */ true,
        /* 14 */ false,
        /* 15 */ false,
        /* 16 */ false,
        /* 17 */ true,
        /* 18 */ false,
        /* 19 */ true,
        /* 20 */ false,
        /* 21 */ false,
        /* 22 */ false,
        /* 23 */ true,
        /* 24 */ false,
        /* 25 */ false,
        /* 26 */ false,
        /* 27 */ false,
        /* 28 */ false,
        /* 29 */ true,
        /* 30 */ false,
        /* 31 */ true,
        /* 32 */ false,
        /* 33 */ false,
        /* 34 */ false,
        /* 35 */ false,
        /* 36 */ false,
        /* 37 */ true,
        /* 38 */ false,
        /* 39 */ false,
        /* 40 */ false,
        /* 41 */ true,
        /* 42 */ false,
        /* 43 */ true,
        /* 44 */ false,
        /* 45 */ false,
        /* 46 */ false,
        /* 47 */ true,
        /* 48 */ false,
        /* 49 */ false,
        /* 50 */ false,
        /* 51 */ false,
        /* 52 */ false,
        /* 53 */ true,
        /* 54 */ false,
        /* 55 */ false,
        /* 56 */ false,
        /* 57 */ false,
        /* 58 */ false,
        /* 59 */ true,
        /* 60 */ false,
        /* 61 */ true,
        /* 62 */ false,
        /* 63 */ false,
        /* 64 */ false
    };

    idx_t remaining = n;          /* how many terms still needed */
    seq_t v = 1;                  /* candidate integer */

    while (remaining > 0) {
        /* Count number of 1 bits in v.  Use built‑in popcount
           on the low 64 bits; for this problem v never exceeds
           64‑bit bounds. */
        unsigned ones = __builtin_popcountll((unsigned long long)v);

        /* A number qualifies iff it has a composite number of 1's.
           Composite numbers are ≥4 and not prime. */
        if (ones >= 4 && !is_prime[ones]) {
            if (--remaining == 0)
                return v;
        }
        ++v;
    }

    /* Unreachable, but required for completeness. */
    return v;
}
