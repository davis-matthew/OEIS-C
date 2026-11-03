/*
 * A285201  Stage at which Ken Knowlton's elevator (version 1) reaches floor n
 *          for the first time.
 * https://oeis.org/A285201
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 - n + 2 * Sum_{k=0..n-2} Sum_{j=0..k} k!/j!
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 14, u64 n <= 22, u128 n <= 35,
 *            big any n
 * Validated: standard, pass, 41 terms through n = 396 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return 1;

    idx_t idx = n - 2;               /* maximum k needed */

    /* static arrays for T[k] = Sum_{j=0..k} k!/j! and its prefix sums */
    static seq_t *Tarr = NULL;       /* T[0..max_k] */
    static seq_t *Prefix = NULL;     /* Prefix[0..max_k] */
    static idx_t max_k = 0;          /* largest k computed */

    /* initialise with k = 0 */
    if (!Tarr || !Prefix)
    {
        Tarr = mem_new(seq_t, 1);
        Prefix = mem_new(seq_t, 1);
        Tarr[0] = 1;                /* T0 = 1! / 0! = 1 */
        Prefix[0] = 1;              /* sum_{k=0}^0 T_k = 1 */
        max_k = 0;
    }

    if (idx > max_k)
    {
        idx_t new_size = idx + 1;
        seq_t *new_T = mem_resize(seq_t, Tarr, new_size);
        seq_t *new_P = mem_resize(seq_t, Prefix, new_size);
        if (!new_T || !new_P)
            exit(1);                 /* allocation failure */

        Tarr = new_T;
        Prefix = new_P;

        for (idx_t k = max_k + 1; k <= idx; ++k)
        {
            Tarr[k] = (seq_t)k * Tarr[k - 1] + 1;
            Prefix[k] = Prefix[k - 1] + Tarr[k];
        }
        max_k = idx;
    }

    seq_t pref = Prefix[idx];
    return ((seq_t)2 * pref) - (seq_t)(n - 2);
}
