/*
 * A394735  Primes prime(k) such that prime(k+1) - prime(k) is a perfect
 *          power.
 * https://oeis.org/A394735
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th prime p_k such that the gap p_{k+1}-p_k is a
 *            perfect power (including 1 as 1^e)
 * Range:     u8 n <= 17, u16 n <= 1669, u32 n <= 2500, u64 n <= 2500,
 *            u128 n <= 2500, big any n
 * Helpers:   is_perfect_power_u64, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/next_prime.h"
#include "../utils/is_perfect_power_u64.h"

static seq_t *seq = NULL;          /* buffer of terms found so far */
static size_t seq_cap = 0;          /* capacity of the buffer */
static size_t seq_len = 0;          /* number of terms stored */

static uint64_t next_to_process = 0;/* the next prime to examine */
static bool initialized = false;    /* once initialised, never re‑initialise */

seq_t a(idx_t n)
{
    /* lazy initialisation */
    if (!initialized) {
        seq_cap = 8;
        seq = (seq_t*)malloc(seq_cap * sizeof(seq_t));
        if (!seq) return (seq_t)0;            /* allocation failed – unlikely */
        seq_len = 0;
        next_to_process = 2;                  /* start from the first prime */
        initialized = true;
    }

    /* find at least n terms */
    while (seq_len < (size_t)n) {
        uint64_t pk = next_to_process;         /* current prime to check */
        uint64_t q = next_prime(pk);           /* next prime after pk      */
        uint64_t gap = q - pk;                 /* the gap between pk and q */

        if (is_perfect_power_u64(gap)) {
            /* store pk in the results array */
            if (seq_len == seq_cap) {
                seq_cap *= 2;
                seq = (seq_t*)realloc(seq, seq_cap * sizeof(seq_t));
                if (!seq) return (seq_t)0;    /* reallocation failed */
            }
            seq[seq_len++] = (seq_t)pk;
        }

        next_to_process = q;   /* advance to the next prime */
    }

    return seq[n - 1];
}
