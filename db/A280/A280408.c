/*
 * A280408  Irregular triangle read by rows listing the prime numbers that
 *          appear from the trajectory of n in Collatz Problem.
 * https://oeis.org/A280408
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabf, nonn
 * Formula:   a(n) is the nth prime appearing in the flattened list of *
 *            primes obtained from the Collatz trajectories of the integers *
 *            1,2,3,…, where each trajectory contributes its primes in the
 *            order * they first appear
 * Range:     u8 n <= 98, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

static seq_t *prime_seq = NULL;    /* array of primes collected so far */
static size_t prime_len = 0;       /* number of primes stored                */
static size_t prime_cap = 0;       /* capacity of prime_seq                   */
static idx_t next_n = 1;           /* next integer whose row is to be computed */

/* Append a prime to prime_seq, expanding the array if required.  */
static void append_prime(seq_t p) {
    if (prime_len == prime_cap) {
        size_t new_cap = (prime_cap == 0) ? 16 : prime_cap * 2;
        seq_t *tmp = (seq_t *)realloc(prime_seq, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        prime_seq = tmp;
        prime_cap = new_cap;
    }
    prime_seq[prime_len++] = p;
}

/* Compute the primes that appear in the Collatz trajectory of a
 * single integer n, and append them to prime_seq.  The trajectory
 * stops when it reaches 1 for the second time.  */
static void compute_row(idx_t n) {
    uint64_t m = (uint64_t)n;
    bool first = true;
    while (true) {
        if (is_prime(m))
            append_prime((seq_t)m);

        if (m == 1 && !first)
            break;

        if ((m & 1ULL) == 0) {
            m >>= 1;
        } else {
            /* use 128‑bit arithmetic to avoid overflow */
            unsigned __int128 prod = (unsigned __int128)3 * m + 1;
            m = (uint64_t)prod;
        }
        first = false;
    }
}

/* Public function for the OEIS sequence A280408.  */
seq_t a(idx_t n) {
    /* Ensure we have at least n primes available.  */
    while (prime_len < n) {
        compute_row(next_n);
        ++next_n;
    }
    return prime_seq[n - 1];
}
