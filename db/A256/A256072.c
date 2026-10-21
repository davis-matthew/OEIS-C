/*
 * A256072  Primes that cannot be represented as x*y + x + y, where x >= y >
 *          1.
 * https://oeis.org/A256072
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   primes that cannot be represented as x*y + x + y with x >= y > 1
 * Range:     u8 n <= 7, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   floor_sqrt, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/floor_sqrt.h"

/* ------------------------------------------- */
/*  Cache for the sequence a(n).                */
/* ------------------------------------------- */
static seq_t *seq = NULL;         /* dynamic array of sequence values   */
static size_t cap = 0;            /* current allocated capacity         */
static size_t len = 0;            /* number of elements stored          */
static idx_t  next_candidate = 2; /* next integer to test for primality */

/* ------------------------------------------- */
/*  Find the next prime >= start.              */
/* ------------------------------------------- */
static idx_t next_prime(idx_t start)
{
    if (start <= 2)          /* 2 is the first prime                */
        return 2;
    if ((start & 1u) == 0)   /* make start odd                       */
        ++start;
    for (;; start += 2)
        if (is_prime(start))
            return start;
}

/* ------------------------------------------- */
/*  Grow the cache until it contains at least */
/*  `need` terms.                              */
/* ------------------------------------------- */
static void ensure_cache(idx_t need)
{
    if (len >= (size_t)need)
        return;

    while (len < (size_t)need) {
        /* find next prime */
        idx_t p = next_prime(next_candidate);

        /* test representability: does p+1 have a factorisation d*e with d,e>=3 ? */
        idx_t sum    = p + 1;
        seq_t sq_seq = floor_sqrt((seq_t)sum);
        idx_t sq     = (idx_t)sq_seq;
        bool has_rep = false;

        for (idx_t d = 3; d <= sq && !has_rep; ++d) {
            if (sum % d == 0) {
                idx_t e = sum / d;
                if (e >= 3)
                    has_rep = true;
            }
        }

        /* store p if it cannot be represented */
        if (!has_rep) {
            if (len == cap) {            /* grow capacity */
                cap = cap ? cap * 2 : 8;
                seq = (seq_t *)realloc(seq, cap * sizeof(seq_t));
            }
            seq[len++] = (seq_t)p;
        }

        /* advance to the next candidate for primality testing */
        next_candidate = p + 1;
        if (next_candidate <= 2)
            next_candidate = 3;
        if ((next_candidate & 1u) == 0)
            ++next_candidate;
    }
}

/* ------------------------------------------- */
/*  Public function required by the harness.   */
/* ------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)               /* undefined for 0 – return 0 */
        return (seq_t)0;

    ensure_cache(n);
    return seq[n - 1];
}
