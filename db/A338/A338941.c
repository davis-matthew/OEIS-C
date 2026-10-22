/*
 * A338941  a(1)=1. For n >= 2, let S be the sum of all prime digits in a(1),
 *          a(2), ... a(n-1) and let C be the next nonprime number not already
 *          in the sequence. If S is a prime less than C and is not already a
 *          term of the sequence, a(n) = S. Otherwise, a(n) = C.
 * https://oeis.org/A338941
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   digit_frequencies, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A338941 Sequence Generator
   a(1) = 1
   For n ≥ 2:
     Let S be the sum of all prime digits (2,3,5,7) in a(1) … a(n-1).
     Let C be the next non‑prime number not already in the sequence.
     If S is prime, S < C, and S is not already in the sequence,
         a(n) = S,
     else
         a(n) = C. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#include "../utils/digit_frequencies.h"
#include "../utils/is_prime_big.h"

/* Static cache for sequence values and auxiliary data */
static size_t seq_cap   = 0;          /* allocated capacity of seq array   */
static size_t seq_len   = 0;          /* number of terms currently cached */
static seq_t *seq       = NULL;       /* array of cached terms            */
static seq_t sum_prime_digits = 0;    /* cumulative sum of prime digits   */
static seq_t next_nonprime_candidate = 4; /* next composite not yet used   */

/* Compute the sum of prime digits (2,3,5,7) of a value */
static inline seq_t sumPrimeDigits(seq_t x)
{
    unsigned int freq[10];
    digit_frequencies(x, freq);
    return (seq_t)(freq[2]*2 + freq[3]*3 + freq[5]*5 + freq[7]*7);
}

/* Ensure the sequence array has space for at least 'need' elements. */
static void ensure_capacity(size_t need)
{
    if (need <= seq_cap)
        return;
    size_t new_cap = seq_cap ? seq_cap : 4;
    while (new_cap < need)
        new_cap <<= 1;
    seq_t *new_seq = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_seq) {
        /* out of memory – terminate with abort */
        abort();
    }
    for (size_t i = 0; i < seq_len; ++i)
        new_seq[i] = seq[i];
    free(seq);
    seq = new_seq;
    seq_cap = new_cap;
}

/* Linear search: return true if 'value' already appears */
static bool is_in_seq(seq_t value)
{
    for (size_t i = 0; i < seq_len; ++i)
        if (seq[i] == value)
            return true;
    return false;
}

/* Main sequence generator */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;     /* undefined behaviour for n < 1 */

    /* Lazy initialization on first call */
    if (seq_len == 0) {
        ensure_capacity(8);
        seq[0] = 1;
        seq_len = 1;
        sum_prime_digits = sumPrimeDigits(seq[0]); /* 0 */
        next_nonprime_candidate = 4;
    }

    /* Return cached value if already computed */
    if (n <= (idx_t)seq_len)
        return seq[(size_t)n - 1];

    /* Expand cache to hold all terms up to n */
    ensure_capacity((size_t)n);

    /* Generate terms from current length up to n-1 (zero‑based) */
    for (size_t i = seq_len; i < (size_t)n; ++i) {
        seq_t S = sum_prime_digits;

        /* Find next non‑prime not already in the sequence */
        seq_t C = next_nonprime_candidate;
        while (is_prime_big(C) || is_in_seq(C))
            ++C;

        /* Decide whether to use S or C */
        bool use_S = false;
        if (is_prime_big(S) && S < C && !is_in_seq(S))
            use_S = true;

        seq[i] = use_S ? S : C;

        /* If we used C, advance the candidate for the next step */
        if (!use_S) {
            next_nonprime_candidate = C + 1;
        }

        /* Update the cumulative sum of prime digits */
        sum_prime_digits += sumPrimeDigits(seq[i]);
    }

    seq_len = (size_t)n;
    return seq[(size_t)n - 1];
}
