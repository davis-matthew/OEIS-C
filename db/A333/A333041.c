/*
 * A333041  Odd numbers m such that sigma(m) > sigma(m-1).
 * https://oeis.org/A333041
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 1085, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by brute‑force search for odd m such that σ(m) > σ(m‑1) */

/* Required includes */
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* --------------------------------------------------------------------- */
/* Helper: compute σ(n) for a 64‑bit unsigned integer n.  The result      */
/* is returned as seq_t.  The calculation uses 128‑bit arithmetic to     */
/* avoid overflow for the moderate sized numbers that occur in this     */
/* sequence.  For a big‑integer seq_t this relies on a conversion from  */
/* an unsigned long long to seq_t, which is available in the build.      */
/* --------------------------------------------------------------------- */
static seq_t sigma_uint64(uint64_t n)
{
    unsigned __int128 res = 1;
    uint64_t tmp = n;

    for (uint64_t p = 2; (unsigned __int128)p * p <= (unsigned __int128)tmp; ++p)
    {
        if (tmp % p == 0)
        {
            unsigned __int128 pk   = 1;
            unsigned __int128 term = 1;
            while (tmp % p == 0)
            {
                tmp /= p;
                pk *= p;
                term += pk;
            }
            res *= term;
        }
    }
    if (tmp > 1)
        res *= (1 + (unsigned __int128)tmp);

    return (seq_t)(unsigned long long)res;
}

/* --------------------------------------------------------------------- */
/* State to support memoisation of the sequence.                          */
/* --------------------------------------------------------------------- */
static seq_t  *terms      = NULL;
static size_t  terms_cap  = 0;
static size_t  terms_cnt  = 0;
static uint64_t next_odd  = 3ULL;   /* next odd candidate to test */

/* --------------------------------------------------------------------- */
/* Public sequence function.                                              */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* initialise the dynamic array on first call */
    if (!terms)
    {
        terms_cap = 32;
        terms = (seq_t*)malloc(terms_cap * sizeof(seq_t));
    }

    /* generate terms until we have at least n of them */
    while (terms_cnt < n)
    {
        uint64_t m = next_odd;
        /* guarantee m is odd (next_odd is always odd) */
        seq_t sm  = sigma_uint64(m);
        seq_t sm1 = sigma_uint64(m - 1);

        if (sm > sm1)
        {
            /* grow storage if needed */
            if (terms_cnt >= terms_cap)
            {
                size_t new_cap = terms_cap * 2;
                terms = (seq_t*)realloc(terms, new_cap * sizeof(seq_t));
                terms_cap = new_cap;
            }
            terms[terms_cnt++] = (seq_t)m;
        }

        /* advance to the next odd candidate */
        next_odd = m + 2;
    }

    return terms[n - 1];
}
