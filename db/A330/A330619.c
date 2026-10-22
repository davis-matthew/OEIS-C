/*
 * A330619  Array read by rows: T(n,k) is the number of solutions to the
 *          equation Sum_{i=1..n} x_i^2 == k (mod 4) with x_i in 0..3, where n
 *          >= 0 and 0 <= k <= 3.
 * https://oeis.org/A330619
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 35, u32 n <= 67, u64 n <= 131,
 *            u128 n <= 259, big any n
 * Helpers:   seq_alloc_new_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A330619: a(n) = T(n/4, n mod 4) where T(n,k) is the number of solutions
   to ∑_{i=1..n} x_i^2 ≡ k (mod 4) with x_i ∈ {0,1,2,3}.
   The 4‑vector v(n)=[T(n,0),T(n,1),T(n,2),T(n,3)]ᵀ satisfies
   v(n+1)=M·v(n) with M=[ [2,0,0,2],
                           [2,2,0,0],
                           [0,2,2,0],
                           [0,0,2,2] ] and v(0)=[1,0,0,0]ᵀ. */

/* The function below memoises the rows of the array T.  It grows the
   storage on demand and works for any seq_t type (POD or a C++ class)
   because allocation and deallocation use the library helpers.  */

#include <stddef.h>              /* for size_t */
#include <stdint.h>              /* for uint64_t */
#include <stdlib.h>              /* for abort() */
#include "../utils/seq_alloc_new_array.h"   /* seq_t* seq_alloc_new_array(size_t n) */
#include "../utils/seq_free_array.h"        /* void seq_free_array(seq_t* p) */

/* auxiliary data */
static seq_t *rows = NULL;          /* contiguous block: rows[i*4 + j] = T(i,j) */
static size_t rows_count   = 0;     /* number of rows that have been initialised */
static size_t rows_capacity = 0;    /* current allocated capacity (in rows) */

/* enlarge storage until it holds at least `need` rows */
static void grow_rows_to(size_t need)
{
    if (need < rows_capacity)
        return;  /* already enough */

    size_t new_cap = rows_capacity ? rows_capacity : 1;
    while (new_cap <= need)
        new_cap <<= 1;           /* double until big enough */

    /* allocate new block using the type‑safe allocator */
    seq_t *new_rows = seq_alloc_new_array(new_cap * 4);
    if (!new_rows) abort();   /* should never happen in the tests */

    /* copy existing rows into the new block */
    for (size_t i = 0; i < rows_count * 4; ++i)
        new_rows[i] = rows[i];

    /* free the old block (type‑safe) */
    if (rows) seq_free_array(rows);

    rows = new_rows;
    rows_capacity = new_cap;
}

/* make sure that rows[0…n] are computed */
static void ensure_rows(size_t n)
{
    if (n < rows_count) return;

    grow_rows_to(n);

    /* initialise the very first row, if we just allocated storage */
    if (rows_count == 0) {
        rows[0] = (seq_t)1;
        rows[1] = (seq_t)0;
        rows[2] = (seq_t)0;
        rows[3] = (seq_t)0;
        rows_count = 1;
    }

    /* compute all missing rows from rows_count to n inclusive */
    for (size_t i = rows_count; i <= n; ++i) {
        size_t prev = i - 1;
        seq_t a0 = rows[4*prev + 0];
        seq_t a1 = rows[4*prev + 1];
        seq_t a2 = rows[4*prev + 2];
        seq_t a3 = rows[4*prev + 3];

        seq_t two_a0 = a0 + a0;
        seq_t two_a1 = a1 + a1;
        seq_t two_a2 = a2 + a2;
        seq_t two_a3 = a3 + a3;

        rows[4*i + 0] = two_a0 + two_a3;  /* 2*a0 + 2*a3 */
        rows[4*i + 1] = two_a0 + two_a1;  /* 2*a0 + 2*a1 */
        rows[4*i + 2] = two_a1 + two_a2;  /* 2*a1 + 2*a2 */
        rows[4*i + 3] = two_a2 + two_a3;  /* 2*a2 + 2*a3 */
    }

    rows_count = n + 1;
}

/* --- public API ----------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* n can be treated as an unsigned integer (uint64_t).  Convert
       to size_t only when the value is known to be safe for the
       expected problem size (here up to about 10000 rows). */
    size_t row = (size_t)(n / 4);
    size_t col = (size_t)(n % 4);

    ensure_rows(row);
    return rows[4*row + col];
}
