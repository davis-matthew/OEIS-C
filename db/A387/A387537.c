/*
 * A387537  a(n) = maximum k such that B(n) < B(n+1) < ... < B(n+k-1), where B
 *          = omega (A001221).
 * https://oeis.org/A387537
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8568, u16 n <= 8568, u32 n <= 8568, u64 n <= 8568,
 *            u128 n <= 8568, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = length of the longest strictly increasing run of omega(n) 
   starting at n.  B(n) is simply omega(n).  The function precomputes 
   omega values up to a safe margin and then counts how far the run 
   can extend.  All arithmetic is performed in seq_t (or an 
   equivalent unsigned type).  No hard‑coded data are used.  */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static size_t table_cap = 0;                  /* size of precomputed arrays */
static unsigned char *omega_table = NULL;     /* omega(n) for 0 <= n <= cap      */
static unsigned char *B_table     = NULL;     /* B(n) = omega(n)                 */

/* Build omega and B tables at least up to 'up_to' (inclusive). */
static void precompute(size_t up_to)
{
    if (up_to <= table_cap) return;

    /* Reallocate arrays to the new size (plus one for index 0). */
    unsigned char *new_omega = (unsigned char *)realloc(omega_table, up_to + 1);
    unsigned char *new_B     = (unsigned char *)realloc(B_table,     up_to + 1);
    if (!new_omega || !new_B) abort();

    omega_table = new_omega;
    B_table     = new_B;

    /* Initialise to zero. */
    for (size_t i = 0; i <= up_to; ++i) omega_table[i] = 0;

    /* Sieve for the number of distinct prime factors (omega). */
    for (size_t p = 2; p <= up_to; ++p) {
        if (omega_table[p] == 0) {                 /* p is prime */
            for (size_t j = p; j <= up_to; j += p) {
                ++omega_table[j];
            }
        }
    }

    /* B(n) = omega(n) for all n. */
    for (size_t i = 0; i <= up_to; ++i) {
        B_table[i] = omega_table[i];
    }

    table_cap = up_to;
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;                /* undefined for n = 0 */

    /* Ensure we have enough precomputed values.  The margin covers
       most runs; if a longer run occurs we extend on demand. */
    size_t margin = 200;                       /* safety margin */
    precompute((size_t)n + margin);

    size_t cur   = (size_t)n;
    size_t len   = 1;                          /* at least the term at n */

    while (1) {
        if (cur + 1 > table_cap) {
            /* Need more values; extend the tables. */
            size_t new_cap = table_cap * 2;
            precompute(new_cap > cur + 1 ? new_cap : cur + 1);
        }
        if (B_table[cur] < B_table[cur + 1]) {
            ++len;
            ++cur;
        } else break;
    }
    return (seq_t)len;
}
