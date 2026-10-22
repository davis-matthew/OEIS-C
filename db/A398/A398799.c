/*
 * A398799  Start with a list of positive integers, L. At step n, swap L(n)
 *          with the least multiple of L(n)+1 that has not previously been
 *          used as a swap target. The sequence consists of the successive
 *          swap targets divided by 2.
 * https://oeis.org/A398799
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   The sequence a(n) is defined by the swap process described in
 *            the OEIS entry for A398799. a(n) is the nth swap target divided
 *            by 2
 * Range:     u8 n <= 158, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

static seq_t *pos = NULL;          /* position → current value (1‑based)   */
static size_t pos_cap = 0;         /* capacity of pos array              */
static uint8_t *used = NULL;       /* used[i] = 1 if i has been a target */
static size_t used_cap = 0;        /* capacity of used array             */
static seq_t *a_vals = NULL;       /* sequence values, 1‑based           */
static size_t a_cap = 0;           /* capacity of a_vals                 */
static idx_t last_step = 0;        /* highest step computed so far        */

/* Ensure the pos array can hold indices up to n (inclusive).  Newly
   allocated entries are initialised to their index value. */
static void ensure_pos(size_t n)
{
    if (n < pos_cap) return;
    size_t new_cap = pos_cap == 0 ? 256 : pos_cap;
    while (new_cap <= n) new_cap <<= 1;
    seq_t *new_pos = (seq_t *)realloc(pos, new_cap * sizeof(seq_t));
    if (!new_pos) abort();
    pos = new_pos;
    for (size_t i = pos_cap; i < new_cap; ++i)
        pos[i] = (seq_t)i;
    pos_cap = new_cap;
}

/* Ensure the used array can hold indices up to n (inclusive).  Newly
   allocated entries are set to 0. */
static void ensure_used(size_t n)
{
    if (n < used_cap) return;
    size_t new_cap = used_cap == 0 ? 256 : used_cap;
    while (new_cap <= n) new_cap <<= 1;
    uint8_t *new_used = (uint8_t *)realloc(used, new_cap);
    if (!new_used) abort();
    used = new_used;
    for (size_t i = used_cap; i < new_cap; ++i)
        used[i] = 0;
    used_cap = new_cap;
}

/* Ensure the a_vals array can hold indices up to n (inclusive). */
static void ensure_a(size_t n)
{
    if (n < a_cap) return;
    size_t new_cap = a_cap == 0 ? 256 : a_cap;
    while (new_cap <= n) new_cap <<= 1;
    seq_t *new_a = (seq_t *)realloc(a_vals, new_cap * sizeof(seq_t));
    if (!new_a) abort();
    a_vals = new_a;
    a_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n<1, but guard */
    if (!pos)                      /* initialise storage on first call */
    {
        ensure_pos(0);
        ensure_used(0);
        ensure_a(0);
    }

    /* Compute all steps up to n if not yet done. */
    for (idx_t step = last_step + 1; step <= n; ++step)
    {
        size_t s = (size_t)step;
        ensure_pos(s);
        seq_t x = pos[s];
        /* base = L(n)+1 */
        idx_t base = (idx_t)x + 1;
        idx_t mult = base;                 /* first candidate multiple */
        /* find the least unused multiple of base */
        for (;;)
        {
            size_t m = (size_t)mult;
            ensure_used(m);
            if (!used[m]) break;
            mult += base;
        }
        size_t m = (size_t)mult;
        related: ;
        used[m] = 1;

        /* Store the sequence value a(step) = mult/2 */
        ensure_a(s);
        seq_t m_seq = (seq_t)mult;
        a_vals[s] = m_seq >> 1;             /* because mult is even (multiple of base+1>=2) */

        /* Perform the swap L(step) <-> L(mult) */
        ensure_pos(m);
        seq_t tmp = pos[s];
        pos[s] = pos[m];
        pos[m] = tmp;
    }

    last_step = n;
    return a_vals[(size_t)n];
}
