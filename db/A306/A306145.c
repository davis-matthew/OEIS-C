/*
 * A306145  Expansion of (1/(1 - x)) * Sum_{k>=0} x^(2*k+1) /
 *          Product_{j=1..2*k+1} (1 - x^j).
 * https://oeis.org/A306145
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 37, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) = sum_{i=0}^n (number of partitions of i into an odd number of parts)
 *  This implements the definition a(n) = A000070(n) - A304620(n) as
 *  the prefix sums of the odd‑part partition function.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static tables for even/odd partitions and their prefix sums     */
    static seq_t *even_arr  = NULL;
    static seq_t *odd_arr   = NULL;
    static seq_t *pref_arr  = NULL;
    static idx_t  allocated = 0;   /* capacity of the tables            */
    static idx_t  max_n     = 0;   /* largest n already computed        */

    /* enlarge allocation if necessary */
    if (n >= allocated)
    {
        idx_t new_cap = (allocated == 0) ? 16 : allocated * 2;
        while (new_cap <= n)
            new_cap <<= 1;                 /* double until enough */

        even_arr = (seq_t *)realloc(even_arr, (size_t)new_cap * sizeof(seq_t));
        odd_arr  = (seq_t *)realloc(odd_arr,  (size_t)new_cap * sizeof(seq_t));
        pref_arr = (seq_t *)realloc(pref_arr, (size_t)new_cap * sizeof(seq_t));

        for (idx_t i = allocated; i < new_cap; ++i)
        {
            even_arr[i] = 0;
            odd_arr[i]  = 0;
            pref_arr[i] = 0;
        }
        allocated = new_cap;
    }

    /* rebuild tables up to n when a larger n is requested */
    if (n > max_n)
    {
        /* reset entries up to n */
        for (idx_t i = 0; i <= n; ++i)
            even_arr[i] = odd_arr[i] = 0;

        even_arr[0] = 1;   /* empty partition has 0 parts (even) */

        for (idx_t part = 1; part <= n; ++part)
        {
            for (idx_t j = part; j <= n; ++j)
            {
                /* Add a part of size 'part'.  It flips the parity.   */
                odd_arr[j] += even_arr[j - part];
                even_arr[j] += odd_arr[j - part];
            }
        }

        /* compute prefix sums of odd partitions */
        pref_arr[0] = odd_arr[0];
        for (idx_t i = 1; i <= n; ++i)
            pref_arr[i] = pref_arr[i - 1] + odd_arr[i];

        max_n = n;
    }
    else if (n > max_n)
    {
        /* extend prefix sums only (odd_arr already contains results) */
        for (idx_t i = max_n + 1; i <= n; ++i)
            pref_arr[i] = pref_arr[i - 1] + odd_arr[i];
        max_n = n;
    }

    return pref_arr[n];
}
