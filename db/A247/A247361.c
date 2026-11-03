/*
 * A247361  Interleave A000861\{0} and A247359: positive numbers with English
 *          name ending in a vowel resp. consonant.
 * https://oeis.org/A247361
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, word
 * Formula:   a(n) = interleaving of the positive integers whose English name
 *            ends in a vowel (odd positions) and those that end in a
 *            consonant (even positions). The function builds the two
 *            sequences lazily upon request
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Validated: partial, pass, 91 terms through n = 91 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/mem.h"

/* Append a value to a dynamic array. */
static void append_to(seq_t **arr, size_t *len, size_t *cap, seq_t val)
{
    if (*len == *cap) {
        size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
        seq_t *tmp = mem_resize(seq_t, *arr, new_cap);
        if (!tmp)
            exit(EXIT_FAILURE);
        *arr = tmp;
        *cap = new_cap;
    }
    (*arr)[*len] = val;
    (*len)++;
}

/* Determine whether the English name of n ends in a vowel.
   Only integer arithmetic is used; no string operations.  The logic
   follows the English naming rules for numbers < 1000 and applies
   recursively to larger numbers via modulus with 1000. */
static bool is_vowel_end(seq_t n)
{
    /* Numbers that are exact multiples of 1000 end with "thousand",
       which concludes with 'n' — a consonant. */
    if (n % 1000 == 0)
        return false;

    /* Work with the last up-to-three‑digit portion of the number. */
    seq_t r = n % 1000;

    if (r < 100) {
        /* Special case: twelve is a vowel; all other teens are consonants. */
        if (r == 12)
            return true;
        if (r >= 10 && r <= 19)
            return false;

        if (r % 10 == 0)            /* tens words end with 'y' */
            return false;

        /* Ones digit determines the ending for single‑digit numbers. */
        seq_t ones = r % 10;
        return (ones == 1 || ones == 2 || ones == 3 ||
                ones == 5 || ones == 9);
    } else {
        /* r is 100..999.  The hundreds word ends with 'd'
           (consonant).  If the low two digits are zero, we already
           returned false above.  Otherwise the ending is determined
           by the value 0..99 below. */
        seq_t low = r % 100;
        if (low == 12)
            return true;
        if (low >= 10 && low <= 19)
            return false;
        if (low % 10 == 0)
            return false;
        seq_t ones = low % 10;
        return (ones == 1 || ones == 2 || ones == 3 ||
                ones == 5 || ones == 9);
    }
}

/* Dynamic arrays for the two interleaved sequences. */
static seq_t *V = NULL;  /* vowel‑ending numbers */
static size_t v_len = 0, v_cap = 0;
static seq_t *C = NULL;  /* consonant‑ending numbers */
static size_t c_len = 0, c_cap = 0;
static seq_t nextNum = 1;   /* smallest number not yet classified */

/* Ensure that V and C contain at least needV and needC elements,
   respectively, extending them if necessary. */
static void ensure_both(size_t needV, size_t needC)
{
    while (v_len < needV || c_len < needC) {
        if (is_vowel_end(nextNum))
            append_to(&V, &v_len, &v_cap, nextNum);
        else
            append_to(&C, &c_len, &c_cap, nextNum);
        ++nextNum;
    }
}

/* Main function of the sequence. */
seq_t a(idx_t n)
{
    size_t oddIndex  = (size_t)((n + 1) / 2);
    size_t evenIndex = (size_t)(n / 2);

    if (n % 2) {         /* odd position – vowel‑ending numbers */
        ensure_both(oddIndex, evenIndex);
        return V[oddIndex - 1];
    } else {             /* even position – consonant‑ending numbers */
        ensure_both(oddIndex, evenIndex);
        return C[evenIndex - 1];
    }
}
