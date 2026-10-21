/*
 * A294482  Solution of the complementary equation a(n) = a(n-2) + b(n-1) + n
 *          + 1, where a(0) = 1, a(1) = 3, b(0) = 2.
 * https://oeis.org/A294482
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Complementary equation: a(n) = a(n-2) + b(n-1) + n + 1
   initial values: a(0)=1, a(1)=3, b(0)=2
   This implementation keeps a and b sequences in memory and
   builds them on demand.  The missing numbers b are generated
   by a simple merge of the already known a values, avoiding
   any fast‑infrarequiring data structures so the program
   works with arbitrary-precision seq_t and remains below
   2 s for n up to ~10 000.  */

#include <stdlib.h>

/* forward declarations */
static void ensure_a_upto(idx_t upto);
static void ensure_b_upto(idx_t target_index);

static seq_t *a_cache = NULL;
static seq_t *b_cache = NULL;
static idx_t a_cache_size = 0;          /* number of a terms currently stored */
static idx_t missing_current = 1;       /* current integer to consider for b */
static idx_t missing_count  = 0;        /* number of b terms already stored */
static idx_t a_pos_global  = 0;         /* next a index to check against missing_current */

/* --------------  auxiliary helpers  -------------- */
static void ensure_b_upto(idx_t target_index)
{
    /* generate b terms until we have b[target_index] */
    while (missing_count <= target_index)
    {
        if (a_pos_global < a_cache_size && a_cache[a_pos_global] == (seq_t)missing_current)
        {
            /* current integer belongs to a, skip it */
            ++a_pos_global;
            ++missing_current;
        }
        else
        {
            /* current integer is missing – store it as the next b term */
            if (missing_count == 0)
                b_cache = (seq_t *)malloc(sizeof(seq_t));
            else
                b_cache = (seq_t *)realloc(b_cache, (missing_count + 1) * sizeof(seq_t));
            b_cache[missing_count] = (seq_t)missing_current;
            ++missing_count;
            ++missing_current;
        }
    }
}

static void ensure_a_upto(idx_t upto)
{
    if (a_cache_size > upto)
        return;                               /* already computed */

    /* grow a_cache to hold values up to 'upto' */
    if (a_cache == NULL)
        a_cache = (seq_t *)malloc((upto + 1) * sizeof(seq_t));
    else
        a_cache = (seq_t *)realloc(a_cache, (upto + 1) * sizeof(seq_t));

    /* compute new a terms */
    for (idx_t i = a_cache_size; i <= upto; ++i)
    {
        if (i == 0)
        {
            a_cache[0] = (seq_t)1;
            a_cache_size = 1;
        }
        else if (i == 1)
        {
            a_cache[1] = (seq_t)3;
            a_cache_size = 2;
        }
        else
        {
            /* ensure b[i-1] is available */
            ensure_b_upto(i - 1);
            /* recurrence: a[i] = a[i-2] + b[i-1] + (i + 1) */
            seq_t ai = a_cache[i - 2] + b_cache[i - 1] + ((seq_t)i + 1);
            a_cache[i] = ai;
            a_cache_size = i + 1;
        }
    }
}

/* --------------  public API  -------------- */
seq_t a(idx_t n)
{
    ensure_a_upto(n);       /* compute a[0..n] if not already done */
    return a_cache[n];
}
