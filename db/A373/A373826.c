/*
 * A373826  Sorted positions of first appearances in the run-lengths
 *          (differing by 0) of the antirun-lengths (differing by > 2) of the
 *          odd primes.
 * https://oeis.org/A373826
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) is the index (1‑based) of the first appearance of each new
 *            run‑length in the run‑lengths of antirun‑lengths of the odd
 *            primes, sorted in increasing order. The algorithm generates odd
 *            primes, forms antiruns (gaps > 2), computes the lengths of those
 *            antiruns, then produces the run‑lengths of e...
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   seen_contains
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/seen_contains.h"

/* Function to grow a dynamic array if needed */
static void grow_array(void **ptr, size_t *cap, size_t elem_size, size_t need)
{
    if (*cap < need)
    {
        size_t newcap = (*cap > 0) ? (*cap << 1) : 16;
        while (newcap < need)
            newcap <<= 1;
        *ptr = realloc(*ptr, newcap * elem_size);
        *cap = newcap;
    }
}

/* Check if a value is already in the seen array (linear search).  */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* Static state across calls */
    static bool initialized = false;
    static seq_t *first_pos = NULL;   /* recorded positions (a(1), a(2), …)        */
    static size_t first_cap = 0, first_len = 0;
    static seq_t *seen_vals = NULL;    /* run‑length values already seen           */
    static size_t seen_cap = 0, seen_len = 0;

    static uint64_t *primes = NULL;     /* all primes generated so far             */
    static size_t primes_cap = 0, primes_len = 0;

    static uint64_t prev_prime = 3;     /* last odd prime processed                */
    static uint64_t antirun_len = 1;    /* number of primes in current antirun       */
    static int prev_L = -1;             /* previous antirun length                  */
    static int run_len_of_L = 0;        /* number of consecutive equal antirun length */
    static idx_t R_index = 0;           /* position in the run‑length sequence      */
    static uint64_t next_candidate = 5; /* next odd number to test for primality    */

    /* One‑time initialization */
    if (!initialized)
    {
        grow_array((void**)&first_pos, &first_cap, sizeof(seq_t), 1);
        grow_array((void**)&seen_vals,  &seen_cap,  sizeof(seq_t), 1);
        grow_array((void**)&primes,    &primes_cap, sizeof(uint64_t), 1);

        primes[0] = 3;
        primes_len = 1;

        initialized = true;
    }

    /* Generate until we have at least n distinct run‑lengths */
    while (first_len < n)
    {
        /* --- Generate next prime (trial division by known primes) --- */
        uint64_t p;
        for (;;)
        {
            bool is_prime = true;
            for (size_t i = 0; i < primes_len; ++i)
            {
                uint64_t q = primes[i];
                if (q > next_candidate / q)      /* q*q would overflow */
                    break;
                if (next_candidate % q == 0)
                {
                    is_prime = false;
                    break;
                }
            }
            if (is_prime)
            {
                p = next_candidate;
                /* store new prime */
                grow_array((void**)&primes, &primes_cap, sizeof(uint64_t), primes_len + 1);
                primes[primes_len++] = p;
                break;
            }
            next_candidate += 2; /* check next odd number */
        }

        /* --- Update antirun based on gap to previous prime --- */
        uint64_t gap = p - prev_prime;
        if (gap > 2)
        {
            antirun_len++;        /* still inside current antirun */
        }
        else
        {
            /* Finish current antirun of length antirun_len */
            int L = (int)antirun_len;
            if (prev_L == L)
            {
                run_len_of_L++;   /* same length continues */
            }
            else
            {
                if (prev_L != -1)
                {
                    /* Output the run‑length of previous equal pair */
                    R_index++;
                    seq_t R_val = (seq_t)run_len_of_L;
                    if (!seen_contains(seen_vals, seen_len, R_val))
                    {
                        /* Record new run‑length */
                        grow_array((void**)&seen_vals, &seen_cap, sizeof(seq_t), seen_len + 1);
                        seen_vals[seen_len++] = R_val;

                        grow_array((void**)&first_pos, &first_cap, sizeof(seq_t), first_len + 1);
                        first_pos[first_len++] = (seq_t)R_index;
                    }
                }
                /* start new run of equal L */
                prev_L = L;
                run_len_of_L = 1;
            }
            antirun_len = 1;          /* start new antirun at p */
        }

        prev_prime = p;
        next_candidate = p + 2;       /* next odd candidate after p */
    }

    return first_pos[n - 1];
}
