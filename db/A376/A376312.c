/*
 * A376312  Run-compression of first differences (A078147) of nonsquarefree
 *          numbers (A013929).
 * https://oeis.org/A376312
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the run‑compression of first differences of
 *            nonsquarefree numbers (A013929), i.e. compress consecutive equal
 *            differences into a single term
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* static cache for the compressed sequence */
static seq_t *compressed = NULL;
static size_t comp_len = 0, comp_cap = 0;

/* last nonsquarefree number examined (as uint64_t) */
static idx_t last_nonsqr = 0;
/* its value as seq_t (for computing differences) */
static seq_t last_nonsqr_val = 0;

/* Test whether n is divisible by a perfect square > 1 */
static bool
is_nonsquarefree(idx_t n)
{
    for (idx_t d = 2; d <= n / d; ++d)
    {
        if (n % (d * d) == 0)
            return true;
    }
    return false;
}

/* Ensure the compressed array has room for at least 'needed' elements */
static void
grow_comp(size_t needed)
{
    if (needed <= comp_cap)
        return;
    size_t new_cap = comp_cap ? comp_cap * 2 : 16;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(compressed, new_cap * sizeof(seq_t));
    compressed = tmp;
    comp_cap = new_cap;
}

/* Generate compressed terms up to index 'target' (1‑based) */
static void
ensure_terms(idx_t target)
{
    if ((size_t)target <= comp_len)
        return;

    while ((size_t)target > comp_len)
    {
        /* Find next nonsquarefree integer after last_nonsqr */
        idx_t x = last_nonsqr + 1;
        for (;;)
        {
            if (is_nonsquarefree(x))
                break;
            ++x;
        }

        seq_t diff = (seq_t)x - last_nonsqr_val;
        if (comp_len == 0)
        {
            grow_comp(1);
            compressed[comp_len++] = diff;
        }
        else if (diff != compressed[comp_len - 1])
        {
            grow_comp(comp_len + 1);
            compressed[comp_len++] = diff;
        }

        last_nonsqr = x;
        last_nonsqr_val = (seq_t)x;
    }
}

/* Public entry point */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* Undefined for n=0; return 0 safely */
    ensure_terms(n);
    return compressed[n - 1];
}
