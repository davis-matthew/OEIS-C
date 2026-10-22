/*
 * A369957  Primes p such that p - 3 and p + 3 are triprimes.
 * https://oeis.org/A369957
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 439, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bigomega, free_seq_array, is_prime, seq_alloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A369957: primes p such that p-3 and p+3 each have exactly 3 prime
   factors (counted with multiplicity). The implementation generates
   primes in increasing order and tests each with the bigomega function. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/bigomega.h"
#include "../utils/seq_alloc.h"
#include "../utils/free_seq_array.h"

/* Static storage for the sequence values. */
static seq_t *seq_vals  = NULL;   /* array of found primes               */
static size_t seq_len   = 0;      /* number of stored primes             */
static size_t seq_cap   = 0;      /* current capacity of seq_vals        */
static uint64_t next_candidate = 5;   /* next odd candidate to test */

/* Ensure that seq_vals has room for at least 'needed' elements. */
static void ensure_capacity(size_t needed)
{
    if (needed <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 8;
    if (new_cap < needed) new_cap = needed;
    seq_t *new_vals = seq_alloc(new_cap);
    for (size_t i = 0; i < seq_len; ++i) new_vals[i] = seq_vals[i];
    seq_vals = new_vals;
    seq_cap = new_cap;
}

/* Append a prime to the sequence, resizing if required. */
static void append_seq(seq_t val)
{
    ensure_capacity(seq_len + 1);
    seq_vals[seq_len++] = val;
}

/* Ensure that seq_vals contains at least 'n' terms of the sequence. */
static void ensure_sequence(idx_t n)
{
    while (seq_len < n) {
        /* Find the next prime. */
        while (true) {
            if (is_prime(next_candidate)) {
                /* Test the triprime condition. */
                if (bigomega(next_candidate - 3) == (seq_t)3 &&
                    bigomega(next_candidate + 3) == (seq_t)3) {
                    append_seq((seq_t)next_candidate);
                }
                next_candidate += 2;   /* move to the next odd number */
                break;                 /* prime processed, continue outer loop */
            }
            next_candidate += 2;         /* skip non‑prime odd numbers */
        }
    }
}

/* Public function required by the harness. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n=0, safe fallback */
    ensure_sequence(n);
    return seq_vals[n - 1];
}
