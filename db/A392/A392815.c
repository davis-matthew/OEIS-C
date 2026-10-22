/*
 * A392815  Numbers k for which A392814(k) > 0.
 * https://oeis.org/A392815
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_special_prime_power
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Property: numbers k such that some prime power p^e in k has e>1 and e≠p.
   The function builds the sequence lazily, caching all computed terms
   so that repeated calls with arbitrary ordering of n work correctly. */
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/has_special_prime_power.h"

/* Smallest prime factor sieve for the numbers 0 .. limit. */
static void compute_spf(idx_t limit, idx_t **out)
{
    idx_t *arr = (idx_t *)malloc((size_t)(limit + 1) * sizeof(idx_t));
    if (!arr) exit(1);                      /* out of memory - abort */
    arr[0] = 0;
    arr[1] = 1;
    for (idx_t i = 2; i <= limit; ++i) arr[i] = 0;

    for (idx_t i = 2; i <= limit; ++i)
    {
        if (arr[i] == 0)
        {
            arr[i] = i;
            if (i <= limit / i)            /* avoid overflow in i*i */
            {
                for (idx_t j = i * i; j <= limit; j += i)
                    if (arr[j] == 0) arr[j] = i;
            }
        }
    }
    *out = arr;
}

/* The main function requested by the problem statement.  */
seq_t a(idx_t n)
{
    static bool initialized = false;
    static idx_t *spf     = NULL;   /* smallest prime factor table   */
    static idx_t spf_limit = 0;     /* current size of the table    */
    static idx_t next      = 2;     /* next integer to test        */
    static seq_t *seq      = NULL;  /* array of found terms         */
    static size_t seq_len  = 0;
    static size_t seq_cap  = 0;

    if (!initialized)
    {
        /* start with a modest sieve; it will grow if necessary */
        spf_limit = 200000;
        compute_spf(spf_limit, &spf);
        seq       = NULL;
        seq_cap   = 0;
        seq_len   = 0;
        next      = 2;
        initialized = true;
    }

    /* Generate terms until we have at least n of them. */
    while (seq_len < n)
    {
        /* Grow the sieve if we run out of pre‑computed SPF data. */
        if (next > spf_limit)
        {
            idx_t new_limit = spf_limit * 2;
            free(spf);
            compute_spf(new_limit, &spf);
            spf_limit = new_limit;
        }

        if (has_special_prime_power(next, spf))
        {
            if (seq_len == seq_cap)
            {
                seq_cap = seq_cap ? seq_cap * 2 : 4;
                seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
                if (!seq) exit(1);
            }
            seq[seq_len++] = (seq_t)next;
        }
        ++next;
    }

    return seq[n - 1];
}
