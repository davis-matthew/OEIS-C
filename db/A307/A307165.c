/*
 * A307165  Numbers k such that the sequence f(0)=f(1)=k, f(x)=(a*b) mod
 *          (a+b+1), where a=f(x-1) and b=f(x-2) is a cycle.
 * https://oeis.org/A307165
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more, hard
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Validated: partial, pass, 6 terms through n = 6 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) : the n-th integer k such that the Abmod sequence
 *        starting with (k,k) is periodic and eventually returns
 *        to the pair (k,k).
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Test whether the pair (k,k) lies on a cycle of the recurrence
 * f(x) = (f(x-1)*f(x-2)) mod (f(x-1)+f(x-2)+1) */
static bool is_cycle(seq_t k)
{
    seq_t a = k, b = k;               /* current state for tortoise and hare */
    seq_t tort_a = a, tort_b = b;      /* tortoise */
    seq_t hare_a = a, hare_b = b;      /* hare    */

    /* Advance tortoise one step and hare two steps until they meet */
    while (true)
    {
        /* tortoise one step */
        seq_t mod1 = tort_a + tort_b + 1;
        seq_t c1   = (tort_a * tort_b) % mod1;
        tort_a = tort_b;
        tort_b = c1;

        /* hare two steps */
        for (int i = 0; i < 2; ++i)
        {
            seq_t mod2 = hare_a + hare_b + 1;
            seq_t c2   = (hare_a * hare_b) % mod2;
            hare_a = hare_b;
            hare_b = c2;
        }

        if (tort_a == hare_a && tort_b == hare_b)
            break;
    }

    /* Find the start of the cycle */
    tort_a = a;
    tort_b = b;
    while (!(tort_a == hare_a && tort_b == hare_b))
    {
        /* tortoise one step */
        seq_t mod1 = tort_a + tort_b + 1;
        seq_t c1   = (tort_a * tort_b) % mod1;
        tort_a = tort_b;
        tort_b = c1;

        /* hare one step */
        seq_t mod2 = hare_a + hare_b + 1;
        seq_t c2   = (hare_a * hare_b) % mod2;
        hare_a = hare_b;
        hare_b = c2;
    }

    /* Cycle includes the starting pair iff the cycle start equals (k,k) */
    return (tort_a == a && tort_b == b);
}

/* Static storage for memoised sequence values */
static seq_t *seq          = NULL;     /* array of found k values      */
static idx_t  terms        = 0;        /* number of values stored     */
static seq_t  next_k       = 0;        /* next candidate to test      */
static size_t cap          = 0;        /* allocated capacity of seq   */

/* Ensure the array has at least 'needed' slots */
static void ensure_capacity(size_t needed)
{
    if (needed <= cap)
        return;
    size_t newcap = (cap == 0) ? 16 : cap * 2;
    while (newcap < needed)
        newcap *= 2;

    seq_t *newp = mem_resize(seq_t, seq, newcap);
    if (!newp)
        abort();                      /* memory exhausted */

    seq  = newp;
    cap  = newcap;
}

/* Main OEIS sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* offset is 1; caller may ignore n==0 */

    while (terms < n)
    {
        if (is_cycle(next_k))
        {
            ensure_capacity(terms + 1);
            seq[terms++] = next_k;
        }
        next_k++;
    }

    return seq[n - 1];
}
