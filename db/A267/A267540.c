/*
 * A267540  Primes p such that p (mod 3) = p (mod 5).
 * https://oeis.org/A267540
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A267540: primes p such that p mod 3 = p mod 5
   (equivalently p mod 15 = 1 or 2).  Computed by incremental
   trial division with caching of primes and of qualifying primes. */

#include <stdlib.h>
#include <stdbool.h>

/* All primes discovered so far (used for trial division). */
static idx_t *prime_list   = NULL;   /* array of primes */
static size_t prime_cap    = 0;
static size_t prime_len    = 0;

/* Primes that satisfy the residue condition. */
static idx_t *good_prime_list = NULL;  /* stores primes as idx_t; cast to seq_t on return */
static size_t good_cap    = 0;
static size_t good_len    = 0;

/* Next candidate to test for primality. */
static idx_t next_candidate = 2;

/* Ensure that good_prime_list contains at least n qualifying primes. */
static void ensure_good(idx_t n)
{
    while (good_len < n) {
        idx_t x = next_candidate;
        bool ok = true;

        if (x < 2) {
            ok = false;
        } else {
            for (size_t i = 0; i < prime_len; ++i) {
                idx_t p = prime_list[i];
                if (p > x / p) break;          /* stop when p*p > x */
                if (x % p == 0) {               /* composite */
                    ok = false;
                    break;
                }
            }
        }

        if (ok) {
            /* Append to prime_list. */
            if (prime_len == prime_cap) {
                prime_cap = prime_cap ? prime_cap * 2 : 16;
                prime_list = (idx_t*)realloc(prime_list,
                                             prime_cap * sizeof(idx_t));
            }
            prime_list[prime_len++] = x;

            /* Check residue condition. */
            if (x % 15 == 1 || x % 15 == 2) {
                if (good_len == good_cap) {
                    good_cap = good_cap ? good_cap * 2 : 16;
                    good_prime_list = (idx_t*)realloc(good_prime_list,
                                                      good_cap * sizeof(idx_t));
                }
                good_prime_list[good_len++] = x;
            }
        }

        /* Advance to the next candidate: after 2, test odd numbers only. */
        if (x == 2)
            next_candidate = 3;
        else
            next_candidate += 2;
    }
}

/* External function requested by the problem statement. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* undefined for n < 1 */
    ensure_good(n);
    return (seq_t)good_prime_list[n - 1];
}
