/*
 * A363162  a(1) = 1, a(2) = 2; for n > 2, a(n) is the smallest positive
 *          number that has not yet appeared that has the same number of
 *          distinct prime divisors as a(n-2) + a(n-1).
 * https://oeis.org/A363162
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 163, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_distinct_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1) = 1, a(2) = 2.
   For n > 2, a(n) is the smallest positive integer not yet used
   that has the same number of distinct prime divisors as a(n-2)+a(n-1). */

#include <stdlib.h>
#include "../utils/count_distinct_primes.h"

/* Storage for computed terms */
static seq_t *seq      = NULL;   /* evaluated terms   */
static idx_t  seq_cap  = 0;      /* capacity of seq   */
static idx_t  seq_size = 0;      /* number of terms computed */

/* Bitmap of numbers that have already appeared */
static unsigned char *used      = NULL;   /* used[i] == 1 iff i already used   */
static idx_t          used_cap  = 0;      /* capacity of used bitmap           */

/* Next candidate to try for a given number of distinct prime divisors (omega) */
static idx_t *next_start   = NULL;   /* next_start[omega] == first candidate not yet tested */
static idx_t  next_start_cap = 0;    /* capacity of next_start array        */

/* Grow an array of seq_t values */
static void ensure_seq_cap(idx_t need)
{
    if (seq_cap >= need) return;
    idx_t new_cap = seq_cap ? seq_cap : 8;
    while (new_cap < need) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    seq = tmp;
    seq_cap = new_cap;
}

/* Grow the used bitmap */
static void ensure_used_cap(idx_t need)
{
    if (used_cap >= need) return;
    idx_t new_cap = used_cap ? used_cap : 1;
    while (new_cap <= need) new_cap <<= 1;
    unsigned char *tmp = (unsigned char *)realloc(used, new_cap * sizeof(unsigned char));
    if (!tmp) abort();
    for (idx_t i = used_cap; i < new_cap; ++i) tmp[i] = 0;
    used = tmp;
    used_cap = new_cap;
}

/* Grow the next_start array */
static void ensure_next_start_cap(idx_t need)
{
    if (next_start_cap >= need) return;
    idx_t new_cap = next_start_cap ? next_start_cap : 8;
    while (new_cap <= need) new_cap <<= 1;
    idx_t *tmp = (idx_t *)realloc(next_start, new_cap * sizeof(idx_t));
    if (!tmp) abort();
    for (idx_t i = next_start_cap; i < new_cap; ++i) tmp[i] = 1;
    next_start = tmp;
    next_start_cap = new_cap;
}

/* Initialise the sequence with the first two values */
static void init_sequence(void)
{
    ensure_seq_cap(2);
    seq[0] = (seq_t)1;
    seq[1] = (seq_t)2;
    seq_size = 2;

    ensure_used_cap(2);
    used[1] = 1;
    used[2] = 1;
}

/* Public function providing the nth term (1‑based) */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety – the API expects a default */

    if (!seq) init_sequence();

    /* If the requested term is already known, return it */
    if (n <= seq_size) return seq[n - 1];

    /* Grow the sequence array to hold the new terms */
    ensure_seq_cap(n);

    /* Compute all needed terms up to n */
    for (idx_t i = seq_size; i < n; ++i)
    {
        /* Compute the target number of distinct prime divisors */
        /* The sum must fit in 64 bits for the helper; grow as needed. */
        uint64_t sum = (uint64_t)seq[i - 1] + (uint64_t)seq[i - 2];
        int target_omega = count_distinct_primes(sum);
        if (target_omega < 0) target_omega = 0;   /* safety */

        ensure_next_start_cap((idx_t)target_omega);
        idx_t cand = next_start[target_omega];

        /* Search for the smallest unused number with the required omega */
        for (;; ++cand)
        {
            ensure_used_cap(cand);
            if (!used[cand] && count_distinct_primes((uint64_t)cand) == target_omega)
            {
                seq[i] = (seq_t)cand;
                used[cand] = 1;
                next_start[target_omega] = cand + 1;
                break;
            }
        }
    }

    seq_size = n;
    return seq[n - 1];
}
