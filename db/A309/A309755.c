/*
 * A309755  Primes with record Euclidean distance from the origin. When
 *          starting rightwards in a grid, turn left after a prime number, if
 *          not walk straight on.
 * https://oeis.org/A309755
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 154, u32 n <= 182, u64 n <= 182,
 *            u128 n <= 182, big any n
 * Helpers:   prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence definition as per A309755:
   Simulate a walk that starts at (0,0) heading right.
   For each step number s = 1,2,3,...:
      • Move one unit in the current direction.
      • After the move, if s is prime, turn left (counter‑clockwise).
      • If s is prime AND the Euclidean distance from the origin at that
        point is strictly larger than all previous distances, the number s
        is recorded as a term of the sequence. */

/* The returned value is the nth such prime (offset = 1). */

#include <stdint.h>
#include <stdbool.h>

static bool is_prime_small(uint64_t n) {
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    for (uint64_t d = 3; d * d <= n; d += 2)
        if (n % d == 0) return false;
    return true;
}

/* Use the library provided primality test when available. 
   Fallback to trial division for small values. */
#ifdef USE_PRIME_TEST
#include "../utils/prime.h"
static bool is_prime(idx_t x) { return is_prime_generic((seq_t)x); }
#else
static bool is_prime(idx_t x) { return is_prime_small((uint64_t)x); }
#endif

seq_t a(idx_t n) {
    if (n == 0) return 0; /* offset starts at 1; undefined for 0 */

    int64_t x = 0, y = 0;          /* current position */
    int dir = 0;                   /* 0:right, 1:up, 2:left, 3:down */
    uint64_t best_d2 = 0;          /* best distance squared seen */
    uint64_t count = 0;            /* number of primes found */
    uint64_t step = 0;             /* current step number */

    /* Loop until we have found n terms. */
    while (count < n) {
        ++step;

        /* Move one unit in the current direction. */
        switch (dir) {
            case 0: ++x; break;
            case 1: ++y; break;
            case 2: --x; break;
            case 3: --y; break;
        }

        /* Compute squared Euclidean distance. */
        uint64_t d2 = (uint64_t)((int64_t)x * (int64_t)x + (int64_t)y * (int64_t)y);

        if (is_prime(step)) {
            if (d2 > best_d2) {
                best_d2 = d2;
                ++count;
                if (count == n) return (seq_t)step;
            }
            /* Turn left: rotate counter-clockwise. */
            dir = (dir + 1) & 3;
        }
        /* If step is composite, we continue straight without turning. */
    }
    /* Should never reach here, but return 0 to satisfy compiler. */
    return 0;
}
