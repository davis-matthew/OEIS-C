/*
 * A400738  For n >= 1, a(n) is the smallest twin prime representable as the
 *          sum of 2*n - 1 consecutive terms of A077800.
 * https://oeis.org/A400738
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, new
 * Formula:   a(n) is the smallest twin prime that can be represented * as the
 *            sum of 2*n-1 consecutive terms of A077800. * The calculation
 *            proceeds by generating the sequence * S of primes occurring in
 *            at least one twin pair (each * twin pair contributes its two
 *            members, allowing a prime to * appear twice if...
 * Range:     u8 n <= 6, u16 n <= 30, u32 n <= 39, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

/* --------------------------------------------------------------------- */
/*  Global state for the generating sequence S (primes in a twin pair)  */
static seq_t *S = NULL;       /* array holding the terms   */
static size_t S_size = 0;      /* number of elements stored */
static size_t S_cap  = 0;      /* allocated capacity        */

/* Prime generation: yields successive odd primes >=3 via Miller–Rabin. */
static uint64_t prime_candidate = 3;   /* next odd number to test */

/* Return next prime >= prime_candidate and advance candidate. */
static uint64_t next_prime(void)
{
    for (;;) {
        if (is_prime_big((seq_t)prime_candidate)) {
            uint64_t p = prime_candidate;
            prime_candidate += 2;          /* skip even numbers */
            return p;
        }
        prime_candidate += 2;
    }
}

/* Ensure that S has at least 'needed' elements.  If not, generate more
 * terms by scanning primes p and adding the twin pair (p,p+2) whenever
 * both are prime.  The duplicated primes that belong to two pairs are
 * automatically inserted twice because the loop visits each prime once. */
static void ensure_S_size(size_t needed)
{
    while (S_size < needed) {
        uint64_t p = next_prime();               /* next prime p        */
        if (is_prime_big((seq_t)p + 2)) {         /* forms a twin pair?  */
            /* grow if necessary */
            if (S_size + 2 > S_cap) {
                size_t new_cap = S_cap ? S_cap * 2 : 64;
                seq_t *tmp = (seq_t *)realloc(S, new_cap * sizeof(seq_t));
                if (!tmp) exit(EXIT_FAILURE);
                S      = tmp;
                S_cap  = new_cap;
            }
            S[S_size++] = (seq_t)p;
            S[S_size++] = (seq_t)(p + 2);
        }
        /* otherwise ignore, just move on to next prime */
    }
}

/* Helper: test whether v is a twin prime (v or v±2 is part of a twin). */
static bool is_twin_prime(seq_t v)
{
    if (!is_prime_big(v))
        return false;
    if (v >= 3 && is_prime_big(v - 2))
        return true;
    if (is_prime_big(v + 2))
        return true;
    return false;
}

/* Main sequence function.  n is a 1‑based index. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;            /* out of bounds – undefined */
    idx_t L = 2 * n - 1;                    /* window length   */

    /* Make sure we have at least L elements (for the first window). */
    ensure_S_size((size_t)L);

    /* Compute the sum of the first window. */
    seq_t sum = 0;
    for (idx_t i = 0; i < L; ++i)
        sum += S[i];

    /* Check if it is a twin prime. */
    if (is_twin_prime(sum))
        return sum;

    /* Slide the window until the first twin prime sum appears.
     * Because the sequence S is non‑decreasing, sums are monotone
     * non‑decreasing, so the first such sum is the smallest. */
    idx_t start = 1;                       /* start index of window */
    for (;;) {
        /* Ensure that the element needed for the new window exists. */
        ensure_S_size((size_t)(start + L));

        /* Update running sum: remove leftmost, add new rightmost. */
        sum -= S[start - 1];
        sum += S[start + L - 1];

        if (is_twin_prime(sum))
            return sum;
        ++start;
    }
}
