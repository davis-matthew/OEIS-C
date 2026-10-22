/*
 * A356680  The lexicographically earliest infinite squarefree sequence of
 *          nonnegative integers that starts with 1, 2.
 * https://oeis.org/A356680
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* // Lexicographically earliest infinite square‑free sequence
 * // starting with 1, 2.  The sequence is built greedily:
 * // at each step append the smallest non‑negative integer that
 * // does not create a square (a block XX) ending at the new
 * // position.  The function is memoised; it works for calls in
 * // any order, and works for any seq_t/idx_t provided by the
 * // framework. */
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* static storage for the sequence ------------------------------------ */
static seq_t *seq  = NULL;   /* dynamic array of values           */
static size_t cap  = 0;      /* current allocated capacity        */
static size_t len  = 0;      /* number of elements present       */

/* Ensure the array can hold at least n elements. */
static void
ensure_capacity(size_t n)
{
    if (n <= cap)
        return;
    size_t new_cap = cap < 16 ? 16 : cap * 2;
    while (new_cap < n)
        new_cap <<= 1;
    seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    cap = new_cap;
}

/* Test whether appending candidate at position pos (0‑based)
 * would create a square ending at that position. */
static bool
creates_square(idx_t pos, seq_t candidate)
{
    /* The maximum block length that can end at pos is
       floor((pos+1)/2).  For odd pos this is larger by one
       than pos/2. */
    idx_t maxL = (pos + 1) / 2;
    for (idx_t L = 1; L <= maxL; ++L)
    {
        bool ok = true;
        for (idx_t i = 0; i < L; ++i)
        {
            idx_t idx1 = pos - 2 * L + 1 + i;   /* first block index   */
            idx_t idx2 = pos - L + 1 + i;       /* second block index  */
            seq_t val2 = (i == L - 1) ? candidate : seq[idx2];
            if (seq[idx1] != val2)
            {
                ok = false;
                break;
            }
        }
        if (ok)
            return true;        /* square of length L would appear */
    }
    return false;
}

/* Generate terms up to and including index n (1‑based). */
static void
grow_to(idx_t n)
{
    if (len >= (size_t)n)
        return;   /* already long enough */

    /* initial seed 1, 2 */
    if (len == 0)
    {
        ensure_capacity(2);
        seq[0] = (seq_t)1;
        seq[1] = (seq_t)2;
        len   = 2;
    }

    while (len < (size_t)n)
    {
        idx_t pos = (idx_t)len;        /* position to fill (0‑based) */
        seq_t cand = (seq_t)0;
        /* find smallest integer that keeps the sequence squarefree */
        while (creates_square(pos, cand))
            cand++;
        ensure_capacity(len + 1);
        seq[len++] = cand;
    }
}

/* Return the n‑th term (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                /* out of range, harmless */
    grow_to(n);
    return seq[n - 1];
}
