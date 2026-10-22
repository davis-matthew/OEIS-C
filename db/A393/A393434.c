/*
 * A393434  a(0) = 0; thereafter a(n) is the least integer (in absolute value)
 *          not yet in the sequence such that the absolute difference between
 *          a(n-1) and a(n) is a factorial number k!, k >= 1; in case of a
 *          tie, preference is given to the positive value.
 * https://oeis.org/A393434
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, changed
 * Formula:   not stated
 * Range:     i8 n <= 72, i16 n <= 9000, i32 n <= 9000, i64 n <= 9000,
 *            i128 n <= 9000, big any n
 * Helpers:   alloc_seq_array, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A393434: Heraclitus transform of the factorials
   a(0) = 0; for n>0, a(n) is the least integer not yet in the sequence
   such that |a(n-1)-a(n)| is a factorial k!, k≥1; in case of a tie
   preference is given to the positive value.  The implementation uses
   an iterative construction with explicit search over the factorial
   differences. */

#include <stdbool.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/seq_realloc.h"

/* Helper: absolute value for seq_t. */
static inline seq_t abs_seq(seq_t x)
{
    return x < (seq_t)0 ? -x : x;
}

/* Search whether x has already occurred in the sequence. */
static bool used_contains(const seq_t *used, size_t used_len, seq_t x)
{
    for (size_t i = 0; i < used_len; ++i)
        if (used[i] == x)
            return true;
    return false;
}

/* Extend an array to hold at least 'new_cap' elements. */
static void extend_array(seq_t **arr, size_t *cap, size_t new_cap)
{
    if (*cap < new_cap)
    {
        *arr = seq_realloc(*arr, *cap, new_cap);
        *cap = new_cap;
    }
}

/* Global state for the sequence. */
static seq_t   *seq   = NULL;  /* sequence values               */
static size_t  seq_cap = 0;     /* capacity of seq array         */
static size_t  seq_len = 0;     /* number of values stored       */

static seq_t   *used  = NULL;  /* values that have appeared     */
static size_t  used_cap = 0;    /* capacity of used array        */
static size_t  used_len = 0;    /* number of used values         */

static seq_t   *fact  = NULL;  /* factorial table: fact[1]==1!  */
static size_t  fact_cap = 0;   /* capacity of fact array        */
static size_t  fact_len = 0;   /* largest k such that fact[k] known */

static void init_sequence(void)
{
    if (seq_len != 0)                /* already initialised */
        return;

    /* Allocate space for the sequence. */
    extend_array(&seq, &seq_cap, 2);
    seq[0] = (seq_t)0;
    seq_len = 1;
    seq_cap = 2;

    /* Allocate space for the set of used values. */
    extend_array(&used, &used_cap, 2);
    used[0] = (seq_t)0;
    used_len = 1;
    used_cap = 2;

    /* Allocate space for factorials. fact[1] = 1! . */
    extend_array(&fact, &fact_cap, 2);
    fact[1] = (seq_t)1;
    fact_len = 1;
    fact_cap = 2;
}

/* Make sure that factorials 1! … k! are available. */
static void ensure_factorial(size_t k)
{
    if (k <= fact_len)
        return;

    size_t new_cap = k + 1;
    extend_array(&fact, &fact_cap, new_cap);

    /* Compute missing factorials. */
    for (size_t i = fact_len + 1; i <= k; ++i)
        fact[i] = fact[i - 1] * (seq_t)i;

    fact_len = k;
    fact_cap = new_cap;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    init_sequence();

    size_t target = (size_t)n;

    while (seq_len <= target)
    {
        seq_t prev = seq[seq_len - 1];
        seq_t abs_prev = abs_seq(prev);

        bool found = false;
        seq_t best_val = 0;
        seq_t best_abs = 0;

        for (size_t k = 1; ; ++k)
        {
            ensure_factorial(k);
            seq_t f = fact[k];

            /* Candidate prev + f */
            seq_t cand = prev + f;
            if (!used_contains(used, used_len, cand))
            {
                seq_t cand_abs = abs_seq(cand);
                if (!found ||
                    cand_abs < best_abs ||
                    (cand_abs == best_abs && cand > 0 && best_val < 0))
                {
                    best_abs = cand_abs;
                    best_val = cand;
                    found = true;
                }
            }

            /* Candidate prev - f */
            cand = prev - f;
            if (!used_contains(used, used_len, cand))
            {
                seq_t cand_abs = abs_seq(cand);
                if (!found ||
                    cand_abs < best_abs ||
                    (cand_abs == best_abs && cand > 0 && best_val < 0))
                {
                    best_abs = cand_abs;
                    best_val = cand;
                    found = true;
                }
            }

            /* Stop if further factorials cannot improve the best candidate. */
            if (found)
            {
                seq_t limit = abs_prev + best_abs;
                if (f > limit)
                    break;
            }
        }

        /* Append the chosen value. */
        extend_array(&seq, &seq_cap, seq_len + 1);
        seq[seq_len] = best_val;
        ++seq_len;

        /* Record the value as used. */
        extend_array(&used, &used_cap, used_len + 1);
        used[used_len] = best_val;
        ++used_len;
    }

    return seq[target];
}
