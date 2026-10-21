/*
 * A249679  Terms of A007504 divisible by 3.
 * https://oeis.org/A249679
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 46, u32 n <= 9252, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th term of A249679:
 * the cumulative sums of primes that are divisible by 3.
 * a(1) = 0 (sum of zero primes). */
/* seq_t is the integer type supplied by the build (uint8_t … __int128 or
 * an arbitrary‑precision class). */
/* idx_t is uint64_t. */
/* The implementation uses incremental prime generation by trial division
 * and stores both the primes and the accepted sums in static buffers
 * that grow as needed. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* ---------- static state ------------------------------------- */
    static seq_t *terms        = NULL;   /* stored sums that are multiples of 3 */
    static size_t term_cap     = 0;      /* capacity of terms array          */
    static idx_t  term_len     = 0;      /* number of terms currently stored */

    static uint64_t *primes    = NULL;   /* list of primes found so far       */
    static size_t   primes_cap = 0;      /* capacity of primes array          */
    static idx_t    primes_len = 0;      /* number of primes stored           */

    static uint64_t candidate    = 2;    /* next number to test for prime    */
    static seq_t   current_sum   = 0;    /* running sum of primes            */

    /* ---------- initialise the first term a(1)=0 ------------------ */
    if (term_len == 0) {
        term_cap = 4;
        terms = (seq_t *)malloc(term_cap * sizeof(seq_t));
        terms[0] = 0;          /* a(1) */
        term_len = 1;
    }

    /* ---------- generate until we have at least n terms --------- */
    while (term_len < n) {
        /* ---- test whether candidate is prime ------------------- */
        bool is_prime = true;
        for (idx_t i = 0; i < primes_len && primes[i] <= candidate / primes[i]; ++i) {
            if (candidate % primes[i] == 0) {
                is_prime = false;
                break;
            }
        }

        if (is_prime) {
            /* ---- store the prime and update cumulative sum ------ */
            if (primes_len == primes_cap) {
                primes_cap = (primes_cap == 0) ? 4 : primes_cap << 1;
                primes = (uint64_t *)realloc(primes, primes_cap * sizeof(uint64_t));
            }
            primes[primes_len++] = candidate;

            current_sum += (seq_t)candidate;

            /* ---- if the sum is divisible by 3, store it as a term -- */
            if ((current_sum % (seq_t)3) == 0) {
                if (term_len == term_cap) {
                    term_cap = (term_cap == 0) ? 4 : term_cap << 1;
                    terms = (seq_t *)realloc(terms, term_cap * sizeof(seq_t));
                }
                terms[term_len++] = current_sum;
            }
        }

        /* ---- advance to the next candidate --------------------- */
        candidate = (candidate == 2) ? 3 : candidate + 2;
    }

    return terms[n - 1];
}
