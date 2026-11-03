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
 * Range:     u8 n <= 28, u16 n <= 1569, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 98 terms through n = 8993 (2026-11-03)
 * Helpers:   build_primes, is_prime_u64
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
#include <stdlib.h>
#include <math.h>
#include <stddef.h>
#include "../utils/build_primes.h"
#include "../utils/is_prime_u64.h"

static size_t _prime_limit_for_idx(idx_t n) {
    /* Rough upper bound for the n-th prime using n*(log n + log log n) + 3 */
    if (n < 6) return 15; /* first 6 primes < 15 */
    double nn = (double)n;
    double bound = nn * (log(nn) + log(log(nn))) + 3.0;
    return (size_t)bound;
}

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

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
