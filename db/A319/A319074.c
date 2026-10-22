/*
 * A319074  a(n) is the sum of the first n nonnegative powers of the n-th
 *          prime.
 * https://oeis.org/A319074
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n-1} p_n^k = (p_n^n - 1)/(p_n - 1)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 18,
 *            big any n
 * Helpers:   sieve_up_to
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Where p_n is the n‑th prime (n ≥ 1). */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/sieve_up_to.h"

/* ---------- prime storage ---------- */

static uint64_t *prime_array = NULL;   /* 0‑based array of primes */
static size_t   prime_count  = 0;      /* number of primes stored */

/* ---------- helper: sieve up to given limit ---------- */

/* ---------- helper: ensure we have at least n primes ---------- */

static void ensure_prime_count(idx_t n)
{
    if ((size_t)n <= prime_count) return;

    idx_t estimate = (n < 6) ? 15 : n * 20;   /* rough over‑estimate */

    while (1) {
        uint64_t *new_primes = NULL;
        size_t    new_cnt    = 0;
        sieve_up_to(estimate, &new_primes, &new_cnt);

        if (new_cnt >= (size_t)n) {
            free(prime_array);
            prime_array = new_primes;
            prime_count = new_cnt;
            return;
        }

        free(new_primes);
        estimate *= 2;   /* increase bound and try again */
    }
}

/* ---------- helper: integer power (binary exponentiation) ---------- */

static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    seq_t b = base;
    while (exp > 0) {
        if (exp & 1ULL) result = result * b;
        b = b * b;
        exp >>= 1;
    }
    return result;
}

/* ---------- main sequence function ---------- */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* safety guard; sequence defined for n ≥ 1 */

    ensure_prime_count(n);

    /* primes array is 0‑based, n‑th prime is at index n-1 */
    seq_t p = (seq_t)prime_array[n - 1];

    seq_t p_pow_n = pow_seq(p, n);
    seq_t numerator   = p_pow_n - (seq_t)1;
    seq_t denominator = p - (seq_t)1;
    seq_t result      = numerator / denominator;

    return result;
}
