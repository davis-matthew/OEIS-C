/*
 * A376003  Positive integers k such that each digit of k^2 is a factor of k.
 * https://oeis.org/A376003
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A376003: a(n) is the n-th positive integer k such that
 * every decimal digit of k^2 is a factor of k.  The sequence is generated
 * by a simple search: test successive k and keep those that satisfy the
 * condition.  The algorithm stores the generated terms in a static
 * cache so that arbitrary calls to a(idx_t n) are efficient. */
#include <stdbool.h>
#include <stddef.h>

/* Utility: grow array capacity if needed. */
static void ensure_array_capacity(size_t needed, size_t *cap, void **buf,
                                  size_t elem_size)
{
    if (needed <= *cap)
        return;
    size_t new_cap = (*cap == 0 ? 8 : *cap * 2);
    if (new_cap < needed)
        new_cap = needed;
    *buf = realloc(*buf, new_cap * elem_size);
    *cap = new_cap;
}

/* Test whether k satisfies the digit condition. */
static bool is_valid_k(seq_t k)
{
    seq_t sq = k * k;
    if (sq == 0)
        return false;
    while (sq != 0)
    {
        seq_t d = sq % 10;   /* current least–significant digit */
        if (d == 0 || k % d != 0)   /* 0 cannot divide k */
            return false;
        sq /= 10;
    }
    return true;
}

seq_t a(idx_t n)
{
    static seq_t *terms = NULL;          /* cached values */
    static size_t terms_cap = 0;
    static size_t terms_len = 0;
    static seq_t next_k = 1;              /* next k to test */

    if (n == 0)                /* undefined, but avoid under‑flow */
        return 0;

    /* Extend cache until we have at least n terms. */
    while (terms_len < (size_t)n)
    {
        seq_t k = next_k;
        ++next_k;  /* increment for next iteration */

        if (is_valid_k(k))
        {
            /* add k to cache */
            ensure_array_capacity(terms_len + 1, &terms_cap,
                                  (void **)&terms, sizeof(seq_t));
            terms[terms_len++] = k;
        }
    }
    return terms[n - 1];
}
