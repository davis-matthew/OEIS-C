/*
 * A350629  Positive even numbers that cannot be written as 3^i (i >= 1) plus
 *          a prime.
 * https://oeis.org/A350629
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n-th positive even number that cannot be written as
 *            3^i + p, where i ≥ 1 and p is a prime
 * Range:     u8 n <= 39, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include "../utils/is_prime_big.h"

#define SIEVE_LIMIT 200000        /* Upper bound for sieving primes */
#define MAX_SEQ      20000        /* Capacity for cached sequence terms */

/* Prime sieve */
static uint8_t is_prime_arr[SIEVE_LIMIT + 1];

/* Powers of 3 up to the sieve limit */
static uint64_t pow3_arr[20];
static size_t pow3_cnt = 0;

/* Sequence cache */
static seq_t seq_arr[MAX_SEQ + 1];   /* 1‑based indexing */
static size_t seq_cnt = 0;           /* Number of terms currently cached */

/* Next even number to examine (static so state is preserved) */
static uint64_t candidate_even = 2;

/* One‑time initialization */
static bool init_done = false;
static void init_sieve(void) {
    /* Eratosthenes sieve */
    memset(is_prime_arr, 1, sizeof is_prime_arr);
    is_prime_arr[0] = is_prime_arr[1] = 0;
    for (uint64_t i = 2; i * i <= SIEVE_LIMIT; ++i)
        if (is_prime_arr[i])
            for (uint64_t j = i * i; j <= SIEVE_LIMIT; j += i)
                is_prime_arr[j] = 0;

    /* Precompute powers of 3 */
    pow3_cnt = 0;
    uint64_t p = 3;
    while (p <= SIEVE_LIMIT) {
        pow3_arr[pow3_cnt++] = p;
        if (p > SIEVE_LIMIT / 3) break;  /* avoid overflow */
        p *= 3;
    }
}

seq_t a(idx_t n) {
    if (!init_done) {
        init_sieve();
        init_done = true;
    }
    if (n < 1) return 0;  /* Sequence starts at index 1 */

    /* Generate terms until we have at least n terms cached */
    while (seq_cnt < n) {
        uint64_t N = candidate_even;        /* Current even number */
        candidate_even += 2;                /* Advance for next iteration */

        bool possible = true;
        if (N >= 5) {                       /* No need to test if N < 5 */
            for (size_t pi = 0; pi < pow3_cnt; ++pi) {
                uint64_t p3 = pow3_arr[pi];
                if (p3 > N - 3) break;     /* N - p3 would be < 3 */
                uint64_t diff = N - p3;    /* diff is odd and ≥ 3 */

                if (diff <= SIEVE_LIMIT) {
                    if (is_prime_arr[diff]) { /* Small prime: found representation */
                        possible = false;
                        break;
                    }
                } else {
                    if (is_prime_big((seq_t)diff)) {
                        possible = false;
                        break;
                    }
                }
            }
        }

        if (possible) {
            if (seq_cnt + 1 > MAX_SEQ) { /* safety check – should not happen */
                return 0;
            }
            seq_cnt++;
            seq_arr[seq_cnt] = (seq_t)N;
        }
    }
    return seq_arr[n];
}
