/*
 * A296562  Numbers k such that k = k_b1 - k_b2, where k_b1 and k_b2 are the
 *          representation of k in two different bases b1 < 10 and b2 < 10.
 * https://oeis.org/A296562
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 17, u32 n <= 18, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Validated: full, pass, 18 terms through n = 18 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: iterate integer k and keep those that satisfy k = D_b1(k) - D_b2(k) for some distinct bases 2 ≤ b1,b2 < 10,
   where D_b(k) is the decimal value obtained from the base‑b representation of k.  The sequence is built incrementally
   and memoised so that any order of calls to a() works. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Compute the decimal value of the base‑b representation of k. */
static seq_t decimal_value(seq_t k, int base)
{
    seq_t temp = k;
    seq_t dec  = 0;
    seq_t mult = 1;
    while (temp > 0)
    {
        int digit = (int)(temp % base);
        temp = temp / base;
        dec += (seq_t)digit * mult;
        mult *= 10;
    }
    return dec;
}

seq_t a(idx_t n)
{
    /* static state for memoisation */
    static seq_t *terms      = NULL;
    static size_t term_cap   = 0;
    static size_t term_len   = 0;
    static seq_t next_k      = 1;

    if (n == 0) return 0;          /* offset is 1 */

    /* generate terms until we reach the requested index */
    while (term_len < n)
    {
        seq_t k   = next_k;
        next_k = next_k + 1;

        /* compute D_b for bases 2..9 */
        seq_t D[8];
        for (int b = 2; b <= 9; ++b)
            D[b-2] = decimal_value(k, b);

        /* check for any ordered pair giving difference k */
        bool ok = false;
        for (int i = 0; i < 8 && !ok; ++i)
            for (int j = 0; j < 8; ++j)
                if (i != j && D[i] >= D[j] && (D[i] - D[j]) == k)
                {
                    ok = true;
                    break;
                }

        if (ok)
        {
            if (term_len == term_cap)
            {
                size_t new_cap = term_cap == 0 ? 16 : term_cap * 2;
                seq_t *tmp = mem_resize(seq_t, terms, new_cap);
                if (!tmp) abort();  /* memory allocation failed */
                terms = tmp;
                term_cap = new_cap;
            }
            terms[term_len++] = k;
        }
    }

    return terms[n-1];
}
