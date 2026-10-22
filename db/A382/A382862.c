/*
 * A382862  Prime numbers whose congruence speed of tetration equals 1.
 * https://oeis.org/A382862
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 43, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A382862 – primes whose congruence speed of tetration equals 1
   (primes p for which p==2 || p==3 || p%150 is one of
   11,13,17,19,23,29,31,37,41,47,53,59,61,67,71,73,77,79,83,89,
   91,97,103,109,113,119,121,127,131,133,137,139). */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* seq_t and idx_t are supplied by the build system. */
seq_t a(idx_t n)
{
    /* a(1)=2, a(2)=3. Return 0 for n==0 (undefined). */
    if (n == 0) return 0;

    /* ------------------------------------------------------------ */
    /* Static storage for primes and filtered primes                */
    /* ------------------------------------------------------------ */
    static uint64_t *primes   = NULL;
    static size_t   primes_cap = 0, primes_len = 0;
    static uint64_t *good     = NULL;
    static size_t   good_cap   = 0, good_len = 0;

    /* --------------------------------------- */
    /* Allowed residues modulo 150             */
    /* --------------------------------------- */
    static bool allowed[150] = {false};
    static bool allowed_init = false;

    /* ------------------------------------------------------------ */
    /* Allocate dynamic arrays on first use                          */
    /* ------------------------------------------------------------ */
    if (!primes_cap) {
        primes_cap = 16;
        primes = (uint64_t*)malloc(primes_cap * sizeof(uint64_t));
    }
    if (!good_cap) {
        good_cap = 16;
        good = (uint64_t*)malloc(good_cap * sizeof(uint64_t));
    }

    /* Ensure capacity when needed                                 */
    if (primes_len + 1 > primes_cap) {
        primes_cap <<= 1;
        primes = (uint64_t*)realloc(primes, primes_cap * sizeof(uint64_t));
    }
    if (good_len + 1 > good_cap) {
        good_cap <<= 1;
        good = (uint64_t*)realloc(good, good_cap * sizeof(uint64_t));
    }

    /* ------------------------------------------------------------ */
    /* Initialize the residue mask once                           */
    /* ------------------------------------------------------------ */
    if (!allowed_init) {
        static const int residues[] = {
            11,13,17,19,23,29,31,37,41,47,
            53,59,61,67,71,73,77,79,83,89,
            91,97,103,109,113,119,121,127,131,133,137,139
        };
        for (size_t i = 0; i < sizeof(residues)/sizeof(residues[0]); ++i)
            allowed[(size_t)residues[i]] = true;
        allowed_init = true;
    }

    /* ------------------------------------------------------------ */
    /* Generate primes and filter them until we have n good primes */
    /* ------------------------------------------------------------ */
    static uint64_t last_checked = 1;   /* start before the first prime */

    while (good_len < n) {
        ++last_checked;

        /* Simple primality test using already known primes */
        bool is_prime = true;
        if (last_checked >= 2) {
            for (size_t i = 0; i < primes_len; ++i) {
                uint64_t p = primes[i];
                if (p * p > last_checked) break;
                if (last_checked % p == 0) {
                    is_prime = false;
                    break;
                }
            }
        } else {
            is_prime = false;
        }
        if (!is_prime) continue;

        /* Store the prime */
        primes[primes_len++] = last_checked;

        /* Accept the prime if it satisfies the condition */
        if (last_checked == 2 || last_checked == 3 ||
            allowed[(size_t)(last_checked % 150)])
        {
            good[good_len++] = last_checked;
        }
    }

    /* ------------------------------------------------------------ */
    /* Return the n‑th good prime (1‑based indexing)                */
    /* ------------------------------------------------------------ */
    return (seq_t)good[n - 1];
}
