/*
 * A360179  a(1) = 1. Thereafter if a(n-1) is a novel term, a(n) = d(a(n-1));
 *          otherwise a(n) = a(n-1) + d(u), where d is the divisor function
 *          A000005 and u is the smallest unstarred prior term (each time we
 *          use a prior term we star it, and starred terms cannot be reused).
 * https://oeis.org/A360179
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look, nice
 * Formula:   OEIS recurrence for A360179: a(1) = 1. For n > 1: if a(n-1) is
 *            novel (not seen among a(1)...a(n-2)) then a(n) = d(a(n-1)) //
 *            number-of-divisors function else let u be the smallest unstarred
 *            prior term (among a(1)...a(n-2)), star u (so it cannot be used
 *            again), a(n) = a(n-1) + d(u)
 * Range:     u8 n <= 5280, u16 n <= 9536, u32 n <= 9536, u64 n <= 9536,
 *            u128 n <= 9536, big any n
 * Helpers:   divisor_count_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/divisor_count_seq.h"

/* --------------------------------------------------------------------- */

/* --------------------------------------------------------------------- */
/* Ensure capacity for an array of elements of size elem_size            */
static void ensure_capacity(idx_t need, void **arr, size_t *cap, size_t elem_size)
{
    if ((size_t)need <= *cap) return;
    size_t newcap = (*cap == 0) ? 64 : *cap * 2;
    if (newcap < (size_t)need) newcap = (size_t)need;
    *arr = realloc(*arr, newcap * elem_size);
    *cap = newcap;
}

/* --------------------------------------------------------------------- */
/* Main sequence function                                                */
seq_t a(idx_t n)
{
    static seq_t *seq    = NULL;    /* seq[1..] holds the terms          */
    static bool *starred = NULL;    /* starred[i] true if a(i) already used as u */
    static size_t seq_cap    = 0;
    static size_t starred_cap = 0;
    static idx_t  seq_len    = 0;   /* largest computed index */

    /* Allocate space for indices up to n (inclusive) */
    ensure_capacity((size_t)(n + 1), (void **)&seq,    &seq_cap,    sizeof(seq_t));
    ensure_capacity((size_t)(n + 1), (void **)&starred, &starred_cap, sizeof(bool));

    /* Initialise first term if needed */
    if (seq_len == 0)
    {
        seq[1]    = (seq_t)1;
        starred[1] = false;
        seq_len = 1;
    }

    /* Compute terms up to n */
    for (idx_t i = seq_len + 1; i <= n; ++i)
    {
        seq_t prev = seq[i - 1];

        /* ---- Check novelty of prev ----------------------------------- */
        bool novel = true;
        for (idx_t j = 1; j < i - 1; ++j)
        {
            if (seq[j] == prev)
            {
                novel = false;
                break;
            }
        }

        if (novel)                                 /* novel term */
        {
            seq[i] = divisor_count_seq(prev);
        }
        else                                       /* repeated term */
        {
            /* Find smallest unstarred prior term */
            seq_t best_val = 0;
            idx_t best_idx = 0;
            bool found = false;
            for (idx_t j = 1; j < i - 1; ++j)
            {
                if (!starred[j])
                {
                    if (!found || seq[j] < best_val ||
                        (seq[j] == best_val && j < best_idx))
                    {
                        best_val = seq[j];
                        best_idx = j;
                        found = true;
                    }
                }
            }
            /* Mark that term as used */
            starred[best_idx] = true;
            seq[i] = prev + divisor_count_seq(best_val);
        }

        /* New term starts unstarred */
        starred[i] = false;
    }

    seq_len = n;
    return seq[n];
}
