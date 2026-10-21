/*
 * A246716  Positive numbers that are not the product of (exactly) two
 *          distinct primes.
 * https://oeis.org/A246716
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the nth positive integer that is NOT the product of two
 *            * distinct primes (i.e., not a square‑free semiprime). This
 *            implementation * checks every integer in turn and returns the
 *            nth qualifying number. The * algorithm is O(n·√m) where m≈a(n);
 *            for n≤10000 it runs well below the * time limit...
 * Range:     u8 n <= 179, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_squarefree_semiprime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_squarefree_semiprime.h"

/* Main sequence function required by the build. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined, guard against misuse */

    uint64_t candidate = 0;
    idx_t    count     = 0;

    while (count < n) {
        candidate++;
        if (!is_squarefree_semiprime(candidate))
            count++;
    }

    return (seq_t)candidate;
}
