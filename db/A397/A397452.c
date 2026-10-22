/*
 * A397452  Number of partitions of n into two 5-smooth numbers.
 * https://oeis.org/A397452
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{(x, y) | x + y = n, x ≤ y, x & y are 5‑smooth}
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* --- static data for 5‑smooth numbers up to the maximum n seen so far --- */
static seq_t *smooth = NULL;         /* sorted list of 5‑smooth numbers */
static size_t smooth_len  = 0;       /* current number of elements in smooth */
static size_t smooth_cap  = 0;       /* allocated capacity of smooth */
static idx_t  max_n_generated = 0;   /* largest n for which we have generated smooth numbers */

/* indices used in the classic Hamming‑number generation algorithm */
static size_t idx2 = 0, idx3 = 0, idx5 = 0;

/*---------------------------------------------------------------*/
/* Generate all 5‑smooth numbers up to 'limit' (inclusive). */
static void
ensure_smooth_up_to(idx_t limit)
{
    /* nothing to do if we already generated enough */
    if (limit <= max_n_generated)
        return;

    /* initialise the list if it is empty */
    if (smooth_len == 0) {
        smooth_cap = 128;
        smooth = (seq_t *)malloc(smooth_cap * sizeof(seq_t));
        smooth_len = 1;
        smooth[0] = (seq_t)1;    /* 1 is 5‑smooth */
        idx2 = idx3 = idx5 = 0;
    }

    /* generate new numbers until the next candidate would exceed the limit */
    while (1) {
        idx_t val2 = (idx_t)smooth[idx2] * 2;
        idx_t val3 = (idx_t)smooth[idx3] * 3;
        idx_t val5 = (idx_t)smooth[idx5] * 5;

        idx_t next = val2;
        if (val3 < next) next = val3;
        if (val5 < next) next = val5;

        if (next > limit)
            break;

        /* grow the array if necessary */
        if (smooth_len == smooth_cap) {
            smooth_cap *= 2;
            smooth = (seq_t *)realloc(smooth, smooth_cap * sizeof(seq_t));
        }
        smooth[smooth_len++] = (seq_t)next;

        if (next == val2) ++idx2;
        if (next == val3) ++idx3;
        if (next == val5) ++idx5;
    }

    max_n_generated = limit;
}

/*---------------------------------------------------------------*/
/* Binary search for a target value in the sorted array 'smooth'. */
static bool
is_5smooth(seq_t target)
{
    size_t lo = 0, hi = smooth_len;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (smooth[mid] == target)
            return true;
        if (smooth[mid] < target)
            lo = mid + 1;
        else
            hi = mid;
    }
    return false;
}

/*---------------------------------------------------------------*/
/* Main function required by the build system. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;

    /* make sure the table of 5‑smooth numbers covers this n */
    ensure_smooth_up_to(n);

    seq_t count = 0;

    for (size_t i = 0; i < smooth_len; ++i) {
        idx_t x = (idx_t)smooth[i];
        if (x > n)
            break;

        idx_t y = n - x;
        if (y < x)           /* already counted the pair (y, x) */
            break;

        if (is_5smooth((seq_t)y))
            ++count;
    }

    return count;
}
