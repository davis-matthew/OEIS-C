/*
 * A373159  Numbers that are not the sum of Pythagorean primes.
 * https://oeis.org/A373159
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 15, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers that are not the sum of Pythagorean primes (primes ≡ 1 mod 4). */
/* The sequence is finite; after the last term all larger integers are
   representable.  For n larger than the number of terms, a(n) = 0. */

/* Note: This code uses is_prime from the utils library to generate primes. */
 
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

/* Forward declaration of helper */
static void compute_terms(void);

/* The cache of sequence terms */
static seq_t *seq_cache = NULL;
static idx_t seq_len   = 0;
static bool seq_ready  = false;

/* ------------------------------------------------------------ */
/* Compute all numbers that are NOT representable as a sum of
   primes p ≡ 1 (mod 4).  The maximum value we need to investigate
   is small – all numbers > 24 are representable, but we use a
   generous limit to be safe. */
static void compute_terms(void)
{
    /* Maximum integer to examine.  A value of 200 is more than enough. */
    const idx_t LIMIT = 200;
    bool representable[LIMIT + 1];
    for (idx_t i = 0; i <= LIMIT; ++i) representable[i] = false;
    representable[0] = true;                   /* empty sum is 0 */

    /* Generate all primes up to LIMIT.  Store those that are ≡ 1 (mod 4). */
    idx_t primes[128], pcount = 0;
    for (idx_t n = 2; n <= LIMIT; ++n) {
        if (is_prime(n)) {
            if (n % 4 == 1)
                primes[pcount++] = n;
        }
    }

    /* Unbounded knapsack: for each coin (prime) add it freely. */
    for (idx_t k = 0; k < pcount; ++k) {
        idx_t coin = primes[k];
        for (idx_t i = coin; i <= LIMIT; ++i) {
            if (representable[i - coin]) representable[i] = true;
        }
    }

    /* Collect the non‑representable positive integers. */
    idx_t temp[128];
    idx_t tcnt = 0;
    for (idx_t i = 1; i <= LIMIT; ++i) {
        if (!representable[i]) temp[tcnt++] = i;
    }

    /* Allocate the final cache.  The sequence is finite. */
    seq_cache = (seq_t *)malloc(tcnt * sizeof(seq_t));
    seq_len   = tcnt;
    for (idx_t i = 0; i < tcnt; ++i) {
        seq_cache[i] = (seq_t)temp[i];
    }
}

/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (!seq_ready) {
        compute_terms();
        seq_ready = true;
    }
    if (n == 0) return (seq_t)0;                  /* offset starts at 1 */
    if (n <= seq_len) return seq_cache[n - 1];
    return (seq_t)0;                              /* beyond the sequence */
}
