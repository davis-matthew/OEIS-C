/*
 * A376593  Second differences of consecutive nonsquarefree numbers (A013929).
 *          First differences of A078147.
 * https://oeis.org/A376593
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = b(n+2) - 2*b(n+1) + b(n) where b(k) is the k‑th
 *            nonsquarefree number (A013929)
 * Range:     i8 n <= 77, i16 n <= 77, i32 n <= 77, i64 n <= 77, i128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* static state for the list of nonsquarefree numbers */
static seq_t *nsq          = NULL;   /* array of nonsquarefree numbers */
static size_t nsq_len      = 0;      /* current number of stored values   */
static size_t nsq_cap      = 0;      /* current capacity of the array     */
static idx_t  cur_candidate = 1;     /* last number examined            */

/* Ensure that the array nsq has at least 'needed' capacity. */
static void ensure_nsq_capacity(size_t needed)
{
    if (nsq_cap >= needed)
        return;
    size_t new_cap = nsq_cap ? nsq_cap << 1 : 64;
    while (new_cap < needed)
        new_cap <<= 1;
    nsq = (seq_t *)realloc(nsq, new_cap * sizeof(seq_t));
    nsq_cap = new_cap;
}

/* Test whether n is NOT squarefree:  return true if some prime square divides n. */
static bool is_nonsquarefree(idx_t n)
{
    /* for n < 4 the result is always false, but the loop below is quick enough */
    for (idx_t p = 2; (p * p) <= n; ++p)
    {
        idx_t pp = p * p;
        if (n % pp == 0)
            return true;      /* divisible by a square, hence not squarefree */
    }
    return false;             /* squarefree */
}

/* Grow the list of nonsquarefree numbers until it contains at least 'need' entries */
static void ensure_nsq_size(size_t need)
{
    if (nsq_len >= need)
        return;
    /* extend the list one by one; candidates are processed in increasing order */
    while (nsq_len < need)
    {
        ++cur_candidate;
        if (is_nonsquarefree(cur_candidate))
        {
            size_t idx = nsq_len;
            ensure_nsq_capacity(idx + 1);
            nsq[idx] = (seq_t)cur_candidate;
            ++nsq_len;
        }
    }
}

seq_t a(idx_t n)
{
    /* a(1) uses the first three nonsquarefree numbers. */
    /* compute at least n+2 terms to guarantee availability for a(n). */
    ensure_nsq_size((size_t)n + 2);

    /* index in 0‑based arrays */
    size_t i = (size_t)n - 1;

    /* a(n) = b(i+2) - 2*b(i+1) + b(i) */
    return (seq_t)nsq[i] - (seq_t)(2 * nsq[i + 1]) + (seq_t)nsq[i + 2];
}
