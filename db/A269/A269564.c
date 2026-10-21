/*
 * A269564  Seven-digit primes whose first, third, fifth, and seventh digits
 *          are the same.
 * https://oeis.org/A269564
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 598, u64 n <= 598, u128 n <= 598,
 *            big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A269564: Seven‑digit primes whose first, third, fifth and seventh
   digits are the same. This function builds all such primes once
   (by brute‑force construction and a primality test) and returns
   the n‑th element, 1‑based. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_small.h"   /* provides bool is_prime_small(seq_t n) */

/* internal cache of the sequence */
static seq_t *prime_list = NULL;   /* dynamically allocated array of primes   */
static size_t prime_count = 0;     /* number of primes stored                 */
static bool initialized = false;   /* has the cache been built?               */

/* helper: build the cache of all qualifying primes */
static void init_primes(void)
{
    if (initialized)
        return;
    initialized = true;

    /* There are at most 9 * 10^3 candidate numbers */
    const size_t MAX_CANDIDATES = 9000;
    seq_t *buffer = (seq_t *)malloc(MAX_CANDIDATES * sizeof(seq_t));
    if (!buffer)
        return;                     /* allocation failed – leave list empty */

    size_t idx = 0;
    /* powers of ten for constructing the 7‑digit number
       d a d b d c d  -> digits 1-7
       positions: 10^6,10^5,10^4,10^3,10^2,10^1,10^0      */
    const seq_t d10s[7] = {1000000ULL, 100000ULL, 10000ULL,
                           1000ULL, 100ULL, 10ULL, 1ULL};

    for (int d = 1; d <= 9; ++d) {          /* first/third/fifth/seventh digit */
        for (int a = 0; a <= 9; ++a) {      /* second digit */
            for (int b = 0; b <= 9; ++b) {  /* fourth digit */
                for (int c = 0; c <= 9; ++c) { /* sixth digit */
                    seq_t val = 0;
                    val += (seq_t)d * d10s[0];
                    val += (seq_t)a * d10s[1];
                    val += (seq_t)d * d10s[2];
                    val += (seq_t)b * d10s[3];
                    val += (seq_t)d * d10s[4];
                    val += (seq_t)c * d10s[5];
                    val += (seq_t)d * d10s[6];

                    if (is_prime_small(val))
                        buffer[idx++] = val;
                }
            }
        }
    }

    if (idx > 0) {
        /* shrink the buffer to fit the actual number of primes */
        seq_t *list = (seq_t *)realloc(buffer, idx * sizeof(seq_t));
        if (list) {
            prime_list = list;
            prime_count = idx;
            return;
        }
    }
    /* If realloc failed or no primes found, clean up */
    free(buffer);
    prime_list = NULL;
    prime_count = 0;
}

/* ------------------------------------------------------------------ */
/* The required OEIS function                                           */
/* ------------------------------------------------------------------ */
seq_t
a(idx_t n)
{
    if (n == 0)
        return 0;                /* sequence offset is 1; n=0 is undefined */

    init_primes();

    if (!prime_list || (size_t)(n - 1) >= prime_count)
        return 0;                /* n out of bounds */

    return prime_list[n - 1];
}
