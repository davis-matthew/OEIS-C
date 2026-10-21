/*
 * A294534  Solution of the complementary equation a(n) = a(n-1) + a(n-2) +
 *          b(n-2) + 2, where a(0) = 1, a(1) = 2, b(0) = 3.
 * https://oeis.org/A294534
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + b(n-2) + 2 a(0) = 1, a(1) = 2, b(0) = 3
 *            (b is the complementary sequence)
 * Range:     u8 n <= 7, u16 n <= 19, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

#define MAX_N 50000  /* maximum n we can handle; should be ≥ any test n */

/* static storage of the sequences */
static seq_t a_arr[MAX_N + 1];
static seq_t b_arr[MAX_N + 1];

/* current state of the computation */
static idx_t current_n   = 0;      /* largest index for which a_arr is known  */
static seq_t next_candidate = 1;   /* current mex */
static idx_t idx_a_used = 0;       /* index of the next a_arr to compare with the mex */
static idx_t idx_b_used = 0;       /* index of the next b_arr to compare with the mex */
static bool initialized = false;

/* Advance next_candidate to the smallest unused integer.
   Uses idx_a_used and idx_b_used to skip over numbers already in a_arr or b_arr. */
static void update_mex(void)
{
    for (;;)
    {
        bool used = false;
        if (idx_a_used <= current_n && a_arr[idx_a_used] == next_candidate)
        {
            used = true;
            ++idx_a_used;
        }
        if (idx_b_used <= current_n && b_arr[idx_b_used] == next_candidate)
        {
            used = true;
            ++idx_b_used;
        }
        if (!used)
            break;
        ++next_candidate;
    }
}

/* One–time initialisation of the first two a-values and the first b-value. */
static void init_once(void)
{
    if (initialized)
        return;
    initialized = true;

    /* base terms */
    a_arr[0] = (seq_t)1;
    a_arr[1] = (seq_t)2;
    b_arr[0] = (seq_t)3;
    current_n = 1;

    /* initialise mex bookkeeping */
    idx_a_used = 0;
    idx_b_used = 0;
    next_candidate = (seq_t)1;
    update_mex();  /* skip 1, 2, 3 */
}

/* Public function requested by the build system */
seq_t a(idx_t n)
{
    init_once();

    /* if we already have the value, just return it */
    if (n <= current_n)
        return a_arr[n];

    /* compute values up to n */
    for (idx_t i = current_n + 1; i <= n; ++i)
    {
        /* recurrence: a(i) = a(i-1) + a(i-2) + b(i-2) + 2 */
        seq_t val = a_arr[i - 1] + a_arr[i - 2] + b_arr[i - 2] + (seq_t)2;
        a_arr[i] = val;
        current_n = i;

        /* a(i) is now used; update mex */
        update_mex();

        /* b(i-1) is the current mex */
        seq_t b_val = next_candidate;
        b_arr[i - 1] = b_val;

        /* move past the used number and update mex again */
        ++next_candidate;
        update_mex();
    }
    return a_arr[n];
}
