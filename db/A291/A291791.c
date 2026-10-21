/*
 * A291791  Numbers n whose trajectory under the iteration of k ->
 *          (sigma(k)+phi(k))/2 reaches a fraction.
 * https://oeis.org/A291791
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 126, u32 n <= 126, u64 n <= 126,
 *            u128 n <= 126, big any n
 * Helpers:   calc_phi, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence of numbers n whose trajectory under
   k -> (sigma(k)+phi(k))/2 eventually reaches an odd sum
   (i.e. a fraction).  The implementation tests each candidate
   by iterating the mapping until an odd sigma+phi is found
   or a cycle is detected.  The result is memoised in a
   dynamically‑grown array so that calls to a(n) are fast for
   any n ≥ 1. */
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/sigma_of.h"
#include "../utils/calc_phi.h"

static seq_t *seq = NULL;       /* array of sequence terms */
static size_t seq_len = 0;      /* number of terms stored */
static size_t seq_cap = 0;      /* allocated capacity of seq */
static idx_t next_candidate = 1;/* candidate to test next */

/* Return true iff starting from x the trajectory reaches a fraction. */
static bool qualifies(seq_t x)
{
    /* maintain a small list of previously seen values to detect cycles */
    static const size_t MAX_STEPS = 256;
    seq_t seen[MAX_STEPS];
    size_t steps = 0;

    while (true) {
        /* cycle detection: if x has appeared before, we are in a cycle
           of even sums and never reach a fraction. */
        for (size_t i = 0; i < steps; ++i) {
            if (seen[i] == x) return false;
        }

        /* compute sigma(x) and phi(x) */
        seq_t sigma = sigma_of((idx_t)x);
        seq_t phi   = calc_phi(x);
        seq_t sum   = sigma + phi;

        /* odd sum → fraction reached */
        if ((sum & 1) != 0) return true;

        /* store x for future cycle detection */
        if (steps < MAX_STEPS)
            seen[steps++] = x;
        else
            return false;           /* too many steps – give up safely */

        /* next value in the trajectory */
        x = sum >> 1;               /* (sigma+phi)/2 */
    }
}

/* Ensure that seq contains at least n terms. */
static void ensure_seq_upto(idx_t n)
{
    while (seq_len < (size_t)n) {
        /* test the next candidate */
        while (!qualifies((seq_t)next_candidate)) {
            ++next_candidate;
        }

        /* append the candidate to the sequence cache */
        if (seq_len == seq_cap) {
            seq_cap = seq_cap ? seq_cap * 2 : 16;
            seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
        }
        seq[seq_len++] = (seq_t)next_candidate++;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    ensure_seq_upto(n);
    return seq[(size_t)(n - 1)];
}
