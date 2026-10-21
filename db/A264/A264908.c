/*
 * A264908  Primes of the form 2^i + 2^j + 2^k - 1, i > j > k > 0.
 * https://oeis.org/A264908
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   is_prime_small, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A264908: primes of the form 2^i + 2^j + 2^k – 1 with i > j > k > 0
   The function returns the n‑th such prime (a(1) = 13). */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/pow2.h"
#include "../utils/is_prime_small.h"

/* Comparator for qsort on arrays of seq_t */
static int seq_cmp(const void *a, const void *b)
{
    const seq_t *x = (const seq_t *)a;
    const seq_t *y = (const seq_t *)b;
    if (*x < *y) return -1;
    if (*x > *y) return 1;
    return 0;
}

/* Return the n‑th prime of the form 2^i+2^j+2^k – 1,  i>j>k>0.  */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined, but guard */

    /* static state for memoisation */
    static seq_t *primes = NULL;          /* array of found primes */
    static size_t primes_len = 0;         /* number of primes stored */
    static size_t primes_cap = 0;         /* capacity of primes array   */
    static idx_t next_i = 2;              /* last processed i */

    /* Generate more primes until we have at least n of them */
    while (primes_len < (size_t)n)
    {
        idx_t i = next_i + 1;            /* next exponent to examine */

        /* number of (j,k) pairs for this i: (i-1 choose 2) */
        size_t pair_count = (size_t)(i - 1) * (size_t)(i - 2) / 2;
        seq_t *temp = NULL;
        size_t temp_len = 0;

        if (pair_count > 0)
        {
            temp = (seq_t *)malloc(pair_count * sizeof(seq_t));
            if (!temp) return 0;         /* out‑of‑memory protection */

            seq_t pow_i = pow2(i);       /* 2^i  (seq_t type) */

            for (idx_t j = 1; j < i; ++j)
            {
                seq_t pow_j = pow2(j);
                for (idx_t k = 1; k < j; ++k)
                {
                    seq_t val = pow_i + pow_j + pow2(k) - (seq_t)1;
                    if (is_prime_small(val))
                    {
                        temp[temp_len++] = val;
                    }
                }
            }

            if (temp_len > 1)
            {
                qsort(temp, temp_len, sizeof(seq_t), seq_cmp);
            }
        }

        /* Ensure primes array has enough capacity for the new block */
        if (primes_cap - primes_len < temp_len)
        {
            size_t new_cap = (primes_cap == 0) ? 4 : primes_cap * 2;
            while (new_cap - primes_len < temp_len)
                new_cap *= 2;

            seq_t *new_ptr = (seq_t *)realloc(primes, new_cap * sizeof(seq_t));
            if (!new_ptr)
            {
                free(temp);
                return 0; /* allocation failure */
            }
            primes = new_ptr;
            primes_cap = new_cap;
        }

        /* Append the sorted block to the primes list */
        for (size_t idx = 0; idx < temp_len; ++idx)
        {
            primes[primes_len + idx] = temp[idx];
        }
        primes_len += temp_len;

        free(temp);
        next_i = i;
    }

    return primes[n - 1];
}
