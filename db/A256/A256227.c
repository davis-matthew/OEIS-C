/*
 * A256227  Naught-y numbers (A011540) that after removing all zeros become
 *          zeroless primes (A038618).
 * https://oeis.org/A256227
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big, strip_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A256227: Naught‑y numbers.
   An integer is accepted if it contains at least one decimal digit 0
   and the number obtained by removing all zeros is a prime that itself
   contains no zeros.  The function returns the n‑th such integer
   (offset 1).  Calls may occur in any order. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>

#include "../utils/is_prime_big.h"
#include "../utils/strip_zero.h"

/* ----- static cache of found terms ----- */
static seq_t *seq = NULL;   /* array holding found terms          */
static size_t seq_len = 0;  /* number of terms currently stored   */
static size_t seq_cap = 0;  /* capacity of seq array              */

/* last integer examined during the incremental search */
static uint64_t cur = 1;    /* start at 1 so we find 10,20,...   */

/* Test whether a 64‑bit integer contains at least one decimal zero. */
static bool contains_zero(uint64_t n)
{
    while (n)
    {
        if ((n % 10) == 0)
            return true;
        n /= 10;
    }
    return false;
}

/* Ensure that the cached sequence contains at least n terms. */
static void ensure_n_terms(idx_t n)
{
    if (seq_len >= (size_t)n)
        return;

    if (seq_cap == 0)
    {
        seq_cap = 8;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        if (!seq)
            exit(1);
    }

    while (seq_len < (size_t)n)
    {
        /* advance to the next integer that contains a zero */
        do
        {
            ++cur;
        } while (!contains_zero(cur));

        /* obtain the zero‑stripped number */
        seq_t stripped;
        strip_zero(cur, &stripped);

        /* accept the candidate if the stripped number is a prime
           that contains no zeros.  is_prime_big handles the required test. */
        if (is_prime_big(stripped))
        {
            if (seq_len == seq_cap)
            {
                seq_cap *= 2;
                seq_t *p = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
                if (!p)
                    exit(1);
                seq = p;
            }
            seq[seq_len++] = (seq_t)cur;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* a(0) is undefined; return 0 for safety */

    ensure_n_terms(n);
    return seq[n - 1];
}
