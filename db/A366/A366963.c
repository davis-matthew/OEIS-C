/*
 * A366963  Numbers whose difference between the largest and smallest digits
 *          is equal to 6.
 * https://oeis.org/A366963
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = number with difference between largest and smallest digit
 *            equals 6
 * Range:     u8 n <= 27, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   append_candidate
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

/* Append a value to a dynamic array, expanding the capacity as needed.
   This function is provided by the utilities. */
#include "../utils/append_candidate.h"

static seq_t next_value = 0;                 /* next integer to test          */
static seq_t *terms = NULL;                  /* dynamic array of terms        */
static size_t terms_cap = 0;                 /* current allocated capacity    */
static size_t terms_len = 0;                 /* number of stored terms        */

/* Check whether a given value has digit range equal to 6. */
static bool is_candidate(seq_t v)
{
    unsigned int min_digit = 10;
    unsigned int max_digit = 0;
    seq_t t = v;
    while (t > 0)
    {
        unsigned int d = (unsigned int)(t % 10);
        if (d < min_digit) min_digit = d;
        if (d > max_digit) max_digit = d;
        t /= 10;
    }
    return (max_digit - min_digit) == 6;
}

/* Generate terms until at least n terms have been produced. */
static void generate_terms_to(idx_t n)
{
    while ((idx_t)terms_len < n)
    {
        next_value++;                                  /* check next integer   */
        if (is_candidate(next_value))
            append_candidate(&terms, &terms_len, &terms_cap, next_value);
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                           /* offset 1: ignore n==0 */
    generate_terms_to(n);
    return terms[n - 1];
}
