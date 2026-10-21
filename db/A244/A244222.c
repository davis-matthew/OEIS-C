/*
 * A244222  Numbers k which have even representation in Greedy Catalan Base,
 *          i.e., where A014418(k) ends with zero.
 * https://oeis.org/A244222
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 1582, u32 n <= 9679, u64 n <= 9679,
 *            u128 n <= 9679, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A244222: Numbers k whose greedy Catalan base representation
   (using unlimited copies of Catalan numbers C_k for k ≥ 2) ends with
   a zero–digit (i.e. no C_0 or C_1 is used).  The value a(n) is the
   n‑th such number, with a(0)=0.  The implementation keeps a dynamic
   cache of the sequence and generates new terms on demand.  Catalan
   numbers are generated on the fly using the recurrence
       C_{m+1} = C_m * 2*(2m+1)/(m+2)
   Their values are stored in an array that grows as needed. */

#include <stdlib.h>

/* ---- Catalan numbers handling ---- */
static seq_t *catalan = NULL;    /* array of Catalan numbers: C_0, C_1, ... */
static size_t catalan_len = 0;     /* number of Catalan numbers stored */
static size_t catalan_cap = 0;     /* allocated capacity for the array */

/* Ensure that the Catalan list contains at least one element greater
   than the given limit.  This guarantees that the largest Catalan
   number not exceeding any value ≤ limit is present in the array. */
static void ensure_catalan_up_to(seq_t limit)
{
    /* Initialize with the first three Catalan numbers if empty. */
    if (catalan_len == 0) {
        size_t init_cap = 8;
        catalan = (seq_t *)malloc(init_cap * sizeof(seq_t));
        if (!catalan) abort();     /* allocation failure -> terminate */
        catalan_cap = init_cap;
        catalan[0] = 1;      /* C_0 */
        catalan[1] = 1;      /* C_1 */
        catalan[2] = 2;      /* C_2 */
        catalan_len = 3;
    }

    /* Grow the array until the last Catalan number exceeds the limit. */
    while (catalan[catalan_len - 1] <= limit) {
        /* Compute next Catalan number C_{n+1}. */
        size_t n = catalan_len - 1;                 /* current index */
        seq_t Cn = catalan[n];
        seq_t term = Cn * ((seq_t)2 * (seq_t)(2 * n + 1));
        term /= (n + 2);                            /* C_{n+1} = C_n * 2*(2n+1)/(n+2) */

        /* Resize the array if necessary. */
        if (catalan_len == catalan_cap) {
            size_t new_cap = catalan_cap * 2;
            if (new_cap == 0) new_cap = 8;
            seq_t *tmp = (seq_t *)realloc(catalan, new_cap * sizeof(seq_t));
            if (!tmp) abort();                     /* allocation failure -> terminate */
            catalan = tmp;
            catalan_cap = new_cap;
        }

        /* Append the new value. */
        catalan[catalan_len++] = term;
    }
}

/* Find the index of the largest Catalan number ≤ x.
   Preconditions: x ≥ 2 and catalog has values up to at least x. */
static size_t largest_catalan_le(seq_t x)
{
    size_t i = catalan_len - 1;
    while (catalan[i] > x) i--;   /* linear search from the end – catalan_len is small */
    return i;
}

/* Test whether k has an even greedy Catalan base representation.
   The greedy algorithm repeatedly subtracts the largest Catalan
   number ≤ the remaining value.  The representation ends with
   a zero digit iff the algorithm never uses C_0 or C_1. */
static int has_even_representation(seq_t k)
{
    if (k == 0) return 1;      /* 0 is included by definition */

    seq_t rem = k;
    while (rem > 0) {
        if (rem < 2) return 0;           /* would need a '1' coin (C_0 or C_1) */
        ensure_catalan_up_to(rem);
        size_t idx = largest_catalan_le(rem);
        if (idx <= 1) return 0;          /* C_0 or C_1 would be used */
        rem -= catalan[idx];
    }
    return 1;
}

/* ---- Sequence a(n) handling ---- */
static seq_t *seq_cache = NULL;     /* dynamic array holding the sequence values */
static size_t seq_len = 0;          /* number of stored terms */
static size_t seq_cap = 0;          /* allocated capacity */
static idx_t next_k = 0;            /* next integer to test for inclusion */

/* Ensure the sequence cache has at least (n + 1) elements. */
static void ensure_sequence_upto(idx_t n)
{
    while (seq_len <= (size_t)n) {
        /* Grow capacity if necessary. */
        if (seq_len == seq_cap) {
            size_t new_cap = seq_cap ? seq_cap * 2 : 8;
            seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();          /* allocation failure -> terminate */
            seq_cache = tmp;
            seq_cap = new_cap;
        }

        /* Test the current integer for inclusion. */
        if (has_even_representation((seq_t)next_k))
            seq_cache[seq_len++] = (seq_t)next_k;

        ++next_k;
    }
}

seq_t a(idx_t n)
{
    ensure_sequence_upto(n);
    return seq_cache[(size_t)n];
}
