/*
 * A365048  a(n) is the number of steps required for the n-th odd prime number
 *          to reach 3 when iterating the following hailstone map: If P+1 == 0
 *          (mod 6), then the next number = smallest prime >= P + (P-1)/2;
 *          otherwise the next number = largest prime <= (P+1)/2.
 * https://oeis.org/A365048
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 1457, u32 n <= 1457, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _prime_limit_for_idx, build_primes, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A365048:  number of hailstone steps for the n‑th odd prime
    Start at the n‑th odd prime (3, 5, 7, …).
    If (P+1) ≡ 0 (mod 6)        → next = smallest prime ≥ P + (P‑1)/2
    Else                         → next = largest prime ≤ (P+1)/2
    Count steps until 3 is reached.                                          */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/_prime_limit_for_idx.h"
#include "../utils/build_primes.h"
#include "../utils/is_prime_big.h"

static idx_t   *prime_list = NULL;   /* primes[0] = 2, primes[1] = 3, ... */
static size_t   prime_count = 0;

/* find the next prime ≥ start (seq_t) */
static seq_t next_prime_seq(seq_t start) {
    seq_t x = start;
    while (!is_prime_big(x)) {
        x = x + 1;
    }
    return x;
}

/* find the largest prime ≤ start (seq_t) */
static seq_t prev_prime_seq(seq_t start) {
    seq_t x = start;
    while (!is_prime_big(x)) {
        x = x - 1;
    }
    return x;
}

seq_t a(idx_t n) {
    /* Ensure we have at least n+1 primes (index 0 is 2) */
    if (prime_count <= (size_t)n) {
        idx_t limit = _prime_limit_for_idx(n);
        while (prime_count <= (size_t)n) {
            if (prime_list) free(prime_list);
            build_primes(limit, &prime_list, &prime_count);
            if (prime_count <= (size_t)n)
                limit *= 2;          /* enlarge if not enough */
        }
    }

    seq_t current = (seq_t)prime_list[n];   /* nth odd prime */
    const seq_t three = 3;
    seq_t steps = 0;

    while (current != three) {
        /* test (P+1) mod 6 == 0 */
        if ( (current + 1) % 6 == 0 ) {
            seq_t cand = current + (current - 1) / 2;
            current = next_prime_seq(cand);
        } else {
            seq_t half = (current + 1) / 2;
            if (is_prime_big(half))
                current = half;
            else
                current = prev_prime_seq(half);
        }
        ++steps;
    }
    return steps;
}
