/*
 * A358401  Difference in number of 0's in first n terms of Van Eck's sequence
 *          and number of primes less than or equal to n.
 * https://oeis.org/A358401
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = A171896(n) - A000720(n)
 * Range:     i8 n <= 74, i16 n <= 74, i32 n <= 74, i64 n <= 74, i128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computes the difference between the number of zeros in the first n
    terms of Van Eck's sequence and the number of primes ≤ n.  */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* --------- static state ----------------------------------------- */
static idx_t  max_precomputed = 0;   /* largest n that has been processed   */
static seq_t *v_seq          = NULL; /* Van Eck terms, 1‑indexed            */
static idx_t *last_occ       = NULL; /* last occurrence of each value       */
static idx_t *zero_prefix    = NULL; /* prefix counts of zeros              */
static idx_t *prime_prefix   = NULL; /* prefix counts of primes ≤ i         */

/* Ensure the dynamic arrays are large enough for an index of 'n'.   */
static void ensure_capacity(idx_t n)
{
    if (n <= max_precomputed)
        return;

    /* New size needs to hold indices 0…n, i.e. n+1 elements.          */
    size_t new_cap = (size_t)(n + 1);

    /* Resize the Van Eck array.                                         */
    v_seq = (seq_t *)realloc(v_seq, new_cap * sizeof(seq_t));
    if (!v_seq) exit(1);

    /* Resize the array that records last occurrences of values.        */
    idx_t *tmp = (idx_t *)realloc(last_occ, new_cap * sizeof(idx_t));
    if (!tmp) exit(1);
    /* New entries are set to 0 (no occurrence yet).                    */
    if (new_cap > max_precomputed)
        memset(tmp + max_precomputed, 0, (new_cap - max_precomputed) * sizeof(idx_t));
    last_occ = tmp;

    /* Resize zero prefix array.                                         */
    zero_prefix = (idx_t *)realloc(zero_prefix, new_cap * sizeof(idx_t));
    if (!zero_prefix) exit(1);

    /* Resize prime prefix array.                                        */
    prime_prefix = (idx_t *)realloc(prime_prefix, new_cap * sizeof(idx_t));
    if (!prime_prefix) exit(1);

    /* Initialise the base of the sequence if this is the first run.      */
    if (max_precomputed == 0) {
        /* Van Eck: the very first term is 0.                           */
        v_seq[1] = 0;
        last_occ[0] = 1;            /* last occurrence of value 0   */
        zero_prefix[0] = 0;
        zero_prefix[1] = 1;         /* one zero in the first term   */
        prime_prefix[0] = 0;
        prime_prefix[1] = 0;        /* 1 is not prime               */
        max_precomputed = 1;
    }

    /* Generate Van Eck terms from the next position up to n.           */
    for (idx_t i = max_precomputed + 1; i <= n; ++i) {
        /* Current value is the previous term.                           */
        seq_t prev_val = v_seq[i - 1];
        /* Van Eck values are non‑negative and < i, so a cast to idx_t
         * is safe.                                                    */
        idx_t val = (idx_t)prev_val;
        idx_t prev_last = last_occ[val];

        seq_t next_val;
        if (prev_last == 0)          /* first occurrence so far      */
            next_val = 0;
        else
            next_val = (seq_t)((i - 1) - prev_last);

        /* Store the term.                                               */
        v_seq[i] = next_val;

        /* Update the last occurrence of the previous value.           */
        last_occ[val] = i - 1;

        /* Update zero prefix.                                           */
        zero_prefix[i] = zero_prefix[i - 1] + (next_val == 0);
    }

    /* Expand prime prefix array to size n+1 by recomputing the sieve
       up to n.  For the target limits (≤ 10 000) this is inexpensive.  */
    {
        unsigned char *is_prime = (unsigned char *)malloc((size_t)(n + 1));
        if (!is_prime) exit(1);
        memset(is_prime, 1, (size_t)(n + 1));
        is_prime[0] = 0;
        if (n >= 1) is_prime[1] = 0;

        for (idx_t p = 2; (idx_t)(p * p) <= n; ++p) {
            if (is_prime[p]) {
                for (idx_t j = p * p; j <= n; j += p)
                    is_prime[j] = 0;
            }
        }

        prime_prefix[0] = 0;
        for (idx_t i = 1; i <= n; ++i)
            prime_prefix[i] = prime_prefix[i - 1] + (is_prime[i] ? 1 : 0);

        free(is_prime);
    }

    /* Update the recorded maximum. */
    max_precomputed = n;
}

/* Public sequence function                                                 */
seq_t a(idx_t n)
{
    if (n == 0)                  /* undefined for offset 1, return 0 safely */
        return 0;

    ensure_capacity(n);
    /* Perform signed subtraction to avoid unsigned wrap‑around.            */
    return (seq_t)((long long)zero_prefix[n] - (long long)prime_prefix[n]);
}
