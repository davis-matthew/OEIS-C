/*
 * A263737  Nonnegative integers that are the difference of two squares but
 *          not the sum of two squares.
 * https://oeis.org/A263737
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A263737: nonnegative integers that are the difference of two squares
   but not the sum of two squares.
   Computation: iterate integers in order, skip those ≡ 2 (mod 4) and
   those representable as a sum of two squares (checked via the
   classical 4k+3–prime‑exponent condition). */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/floor_sqrt.h"   /* provides seq_t floor_sqrt(seq_t) */

/* ---------- Prime table utilities ------------------------------------- */
static uint32_t *prime_buf = NULL;   /* dynamic array of primes           */
static size_t   prime_cap = 0;       /* allocated capacity               */
static size_t   prime_len = 0;       /* number of primes stored           */

/* Ensure we have all primes <= limit.  */
static void
ensure_primes_up_to(idx_t limit)
{
    if (prime_len && (idx_t)prime_buf[prime_len-1] >= limit)
        return;                       /* already enough */

    idx_t start = prime_len ? (idx_t)prime_buf[prime_len-1] + 1 : 2;
    for (idx_t x = start; x <= limit; ++x) {
        bool is_p = true;
        for (size_t i = 0; i < prime_len &&
             (idx_t)prime_buf[i] * (idx_t)prime_buf[i] <= x; ++i) {
            if (x % prime_buf[i] == 0) { is_p = false; break; }
        }
        if (is_p) {
            if (prime_len == prime_cap) {
                prime_cap = prime_cap ? prime_cap * 2 : 16;
                uint32_t *tmp = (uint32_t *)realloc(prime_buf,
                                                    prime_cap * sizeof(uint32_t));
                if (!tmp) exit(1);
                prime_buf = tmp;
            }
            prime_buf[prime_len++] = (uint32_t)x;
        }
    }
}

/* ---------- Sum‑of‑two‑squares test ----------------------------------- */
static bool
is_sum_of_two_squares(seq_t n)
{
    if (n == 0) return true;          /* 0 = 0^2 + 0^2 */

    seq_t tmp = n;
    seq_t sq = floor_sqrt(tmp);            /* floor sqrt */
    idx_t limit = (idx_t)sq + 1;           /* small cast, safe for limits */
    ensure_primes_up_to(limit);

    for (size_t i = 0; i < prime_len; ++i) {
        seq_t p = (seq_t)prime_buf[i];
        if (p * p > tmp) break;
        if (tmp % p == 0) {
            unsigned exp = 0;
            while (tmp % p == 0) { tmp /= p; ++exp; }
            if ( ((p % 4) == 3) && (exp & 1) )
                return false;          /* odd exponent of a prime 3 (mod 4) */
        }
    }
    /* Remaining factor > 1 is prime. */
    if (tmp > 1 && (tmp % 4 == 3))
        return false;
    return true;
}

/* ---------- Main sequence function ----------------------------------- */
seq_t
a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n < 1 */

    idx_t count = 0;
    seq_t m = 0;

    while (1) {
        ++m;
        /* skip numbers ≡ 2 (mod 4) – they cannot be a difference of two squares */
        if ((m & 3) == 2) continue;

        if (is_sum_of_two_squares(m)) continue;   /* skip sums of two squares */

        ++count;
        if (count == n) return m;
    }
}
