/*
 * A379036  Indices of zeros in binary concatenation of primes.
 * https://oeis.org/A379036
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the position (0‑based) of the n‑th zero bit in the
 *            infinite * bitstring obtained by concatenating the binary
 *            representations of all * primes, starting with prime 2. The
 *            function yields a(1)=1, a(2)=5, a(3)=11, * etc. The zero
 *            positions are enumerated exactly as in OEIS A379036. The *
 *            impl...
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   int_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include "../utils/int_sqrt.h"   /* provides idx_t int_sqrt(idx_t n) */

#ifndef BUILDING_ARBITRARY_SUPERINT
/* If an arbitrary‑precision integer type is in use, this macro is defined
 * and the cast operations below will use seq_t appropriately.  The code
 * is otherwise agnostic to the exact size of seq_t. */
#endif

seq_t a(idx_t n)
{
    /* --- Static state for memoisation and incremental processing --- */
    static seq_t *zero_pos = NULL;        /* array of zero positions          */
    static size_t zeros_cap = 0;          /* capacity of zero_pos            */
    static size_t zeros_len = 0;          /* number of zero positions stored */

    static uint64_t *primes = NULL;       /* list of primes generated        */
    static size_t primes_cap = 0;         /* capacity of primes array        */
    static size_t primes_len = 0;         /* number of primes stored         */

    static uint64_t next_candidate = 2;   /* next number to test for prime   */
    static uint64_t bit_index = 0;        /* global bit index counter        */

    /* ----- Initialise arrays on the first call ----- */
    if (zeros_cap == 0) {
        zeros_cap = 16;
        zero_pos = (seq_t *)malloc(zeros_cap * sizeof(seq_t));
        assert(zero_pos != NULL);
    }
    if (primes_cap == 0) {
        primes_cap = 16;
        primes = (uint64_t *)malloc(primes_cap * sizeof(uint64_t));
        assert(primes != NULL);
    }

    /* ----- Generate primes and record zero bits until we have n zeros ----- */
    while (zeros_len < (size_t)n) {
        /* ---------- Check if next_candidate is prime ----------
           We use a simple trial‑division by previously found primes.  */
        bool is_prime = true;
        uint64_t lim = (uint64_t)int_sqrt((idx_t)next_candidate);  /* sqrt */
        for (size_t i = 0; i < primes_len && primes[i] <= lim; ++i) {
            if (next_candidate % primes[i] == 0) {
                is_prime = false;
                break;
            }
        }

        if (is_prime) {
            /* ---- Store the prime ---- */
            if (primes_len == primes_cap) {
                primes_cap *= 2;
                uint64_t *tmp = (uint64_t *)realloc(primes, primes_cap * sizeof(uint64_t));
                assert(tmp != NULL);
                primes = tmp;
            }
            primes[primes_len++] = next_candidate;

            /* ---- Scan the prime's binary representation ---- */
            unsigned bits = 64 - __builtin_clzll(next_candidate);   /* number of bits */
            for (int i = (int)bits - 1; i >= 0; --i) {
                bool bit = ((next_candidate >> i) & 1ULL) != 0;
                if (!bit) {  /* zero bit found */
                    if (zeros_len == zeros_cap) {
                        zeros_cap *= 2;
                        seq_t *tmp = (seq_t *)realloc(zero_pos, zeros_cap * sizeof(seq_t));
                        assert(tmp != NULL);
                        zero_pos = tmp;
                    }
                    zero_pos[zeros_len++] = (seq_t)bit_index;
                }
                ++bit_index;   /* advance global bit index */
            }
        }

        /* ---- Prepare the next candidate number ---- */
        if (next_candidate == 2) {
            next_candidate = 3;
        } else {
            next_candidate += 2;  /* skip even numbers */
        }
    }

    /* ----- Return the requested zero position (1‑based index ---- */
    return zero_pos[(size_t)n - 1];
}
