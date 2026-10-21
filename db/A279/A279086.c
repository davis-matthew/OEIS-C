/*
 * A279086  Number of distinct residues n mod p encountered before the first
 *          repeated residue as p runs through the primes.
 * https://oeis.org/A279086
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm:
   For a given n ≥ 1, compute the residues r_j = n mod p_j for the primes
   p_1 = 2, p_2 = 3, … in increasing order.  Count how many of these
   residues are distinct until the first repeated residue occurs; that
   count is a(n).  The function works for any build – 64‑bit or
   arbitrary‑precision seq_t – because it only needs 64‑bit arithmetic
   for the intermediate residue calculations and the final count.  */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* --------------------------------------------------------------------- */
/* Prime generator (lazy, on‑demand, thread‑unsafe)                      */
/* --------------------------------------------------------------------- */

static uint64_t *primes = NULL;          /* storage for generated primes  */
static size_t   prime_cnt = 0;           /* number of primes stored       */
static size_t   prime_cap = 0;           /* allocated capacity            */
static uint64_t next_candidate = 2;      /* next integer to test         */

/* Ensure that primes array contains at least idx+1 primes (indices 0..idx). */
static void
ensure_prime_at_index(size_t idx)
{
    while (prime_cnt <= idx) {
        uint64_t p = next_candidate;
        bool     is_prime = true;

        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t q = primes[i];
            if (q > p / q)               /* q*q > p, stop testing */
                break;
            if (p % q == 0) {
                is_prime = false;
                break;
            }
        }

        if (is_prime) {
            if (prime_cnt == prime_cap) {
                prime_cap = prime_cap ? prime_cap * 2 : 16;
                uint64_t *tmp = (uint64_t *)realloc(primes,
                                                    prime_cap * sizeof(uint64_t));
                if (!tmp) {                     /* allocation failed */
                    exit(1);
                }
                primes = tmp;
            }
            primes[prime_cnt++] = p;
        }

        /* Advance candidate – skip even numbers after 2 */
        if (p == 2)
            next_candidate = 3;
        else
            next_candidate += 2;
    }
}

/* --------------------------------------------------------------------- */
/* Sequence function a(idx_t n)                                           */
/* --------------------------------------------------------------------- */

seq_t
a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                 /* undefined for n = 0, but return 0 */

    /* Bit‑set of residues seen so far.  Residues lie in [0, n]. */
    size_t seen_bits = n + 1;
    size_t seen_words = (seen_bits + 63) >> 6;   /* 64 bits per word */
    uint64_t *seen = (uint64_t *)calloc(seen_words, sizeof(uint64_t));
    if (!seen)                          /* allocation failure (rare) */
        return (seq_t)0;

    size_t count = 0;                    /* number of distinct residues seen */

    /* Iterate over primes in order, generating them on demand. */
    for (size_t idx = 0; ; ++idx) {
        ensure_prime_at_index(idx);
        uint64_t p = primes[idx];

        uint64_t r = n % p;              /* residue of n modulo p */
        size_t  word  = r >> 6;           /* word index */
        uint64_t mask = (uint64_t)1 << (r & 63);   /* bit mask */

        if (seen[word] & mask) {         /* first repetition encountered */
            free(seen);
            return (seq_t)count;
        }

        seen[word] |= mask;              /* mark this residue as seen  */
        ++count;
    }
}
