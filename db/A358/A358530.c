/*
 * A358530  a(n) = n-th prime prime(k) such that prime(k) - prime(k-1) <
 *          prime(k-1) - prime(k-2).
 * https://oeis.org/A358530
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: the n‑th prime p_k such that
 *    p_k - p_{k-1} < p_{k-1} - p_{k-2}   (k ≥ 3)   */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

/* --------------------------------------------------------------------- */
/* static state: the list of qualifying primes and the last three      */
/* primes used for gap comparison.                                       */
/* --------------------------------------------------------------------- */
static seq_t   *quals = NULL;      /* stored qualifying primes   */
static size_t   qual_cnt = 0;      /* number of stored qualifiers*/
static size_t   qual_cap = 0;      /* allocated capacity          */

static uint64_t p_prev2 = 0;       /* p_{k-2}    */
static uint64_t p_prev1 = 0;       /* p_{k-1}    */
static uint64_t p_curr  = 0;       /* p_{k}      */
static bool     initialized = false;

/* --------------------------------------------------------------------- */
/* produce the next prime > start (deterministic using is_prime).       */
/* This helper is lightweight and only used for the small range needed */
/* for n ≤ 10000.                                                      */
/* --------------------------------------------------------------------- */
static uint64_t next_prime(uint64_t start)
{
    uint64_t x = start + 1;
    while (!is_prime(x))
        ++x;
    return x;
}

/* --------------------------------------------------------------------- */
/* grow the qualifiers array to at least new_cap elements.               */
/* --------------------------------------------------------------------- */
static void grow_qualifiers(size_t new_cap)
{
    if (qual_cap >= new_cap)
        return;
    size_t cap = qual_cap ? qual_cap * 2 : 16;
    while (cap < new_cap)
        cap *= 2;
    seq_t *tmp = (seq_t *)realloc(quals, cap * sizeof(seq_t));
    if (tmp) {
        quals = tmp;
        qual_cap = cap;
    }
}

/* --------------------------------------------------------------------- */
/* generate qualifying primes until we have at least n of them.         */
/* --------------------------------------------------------------------- */
static void ensure_qualifiers(idx_t n)
{
    if (n == 0)
        return;

    if (!initialized) {
        /* seed the prime sequence: 2, 3, 5 ...            */
        p_prev2 = 2;
        p_prev1 = 3;
        p_curr  = next_prime(p_prev1);   /* =5 */
        /* check the first real gap (k = 3); it never qualifies   */
        /* but we need to place primes into the sliding window.   */
        initialized = true;
    }

    while (qual_cnt < n) {
        /* compute next prime */
        uint64_t p_next = next_prime(p_curr);

        /* evaluate gap inequality for k = index of p_next */
        uint64_t gap_curr = p_next - p_curr;      /* p_k - p_{k-1}  */
        uint64_t gap_prev = p_curr - p_prev1;     /* p_{k-1} - p_{k-2} */

        if (gap_curr < gap_prev) {
            grow_qualifiers(qual_cnt + 1);
            if (qual_cap) {           /* we have space */
                quals[qual_cnt++] = (seq_t)p_next;
            }
        }

        /* slide the window: p_{k-2} -> p_{k-1},  p_{k-1} -> p_k,
           p_k -> p_{k+1}                                 */
        p_prev2 = p_prev1;
        p_prev1 = p_curr;
        p_curr  = p_next;
    }
}

/* --------------------------------------------------------------------- */
/* public API: returns the n‑th term of A358530 (1‑based indexing).     */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                /* offset is 1 but guard */

    ensure_qualifiers(n);
    return quals[n - 1];
}
