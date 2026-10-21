/*
 * A290454  Triangle formed by reading the triangle of Eulerian numbers
 *          (A173018) mod 4.
 * https://oeis.org/A290454
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  
 *  Formula:  a(n) = Eulerian triangle A173018 read row‑wise, 
 *  each entry taken modulo 4.  (Row k contains Eulerian numbers
 *  for permutations of k items.)
 */
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

static uint8_t **eul_rows = NULL;   /* pointers to each computed row   */
static size_t    eul_rows_cap = 0;  /* capacity of eul_rows[]         */

/* Ensure that rows 0..r are computed (including r). */
static void
ensure_rows(size_t r)
{
    if (r < eul_rows_cap)
        return;                               /* already done */

    /* Extend the array of row pointers. */
    size_t new_cap = r + 1;
    uint8_t **tmp = (uint8_t**)realloc(eul_rows, new_cap * sizeof(uint8_t*));
    if (!tmp)
        abort();                               /* out of memory */
    eul_rows = tmp;

    size_t start = eul_rows_cap;                /* last computed row + 1 */
    eul_rows_cap = new_cap;

    for (size_t row = start; row <= r; ++row) {
        size_t len = row + 1;
        uint8_t *arr = (uint8_t*)malloc(len);
        if (!arr)
            abort();                           /* out of memory */

        if (row == 0) {
            arr[0] = 1;                        /* A(0,0) = 1 */
        } else {
            arr[0] = 1;                        /* first element */
            arr[row] = 0;                      /* trailing zero for Eulerian numbers */
            for (size_t k = 1; k < row; ++k) {
                unsigned int val =
                    ((k + 1) * eul_rows[row - 1][k] +
                     (row - k) * eul_rows[row - 1][k - 1]) & 3;
                arr[k] = (uint8_t)val;
            }
        }
        eul_rows[row] = arr;
    }
}

/* Main sequence function, external linkage. */
seq_t a(idx_t n)
{
    /* Find the row r that contains index n.
       Cumulative length up to row r‑1 is r*(r+1)/2. */
    size_t r = 0;
    size_t cum = 0;
    while (cum + (r + 1) <= (size_t)n) {
        cum += r + 1;
        ++r;
    }
    size_t k = (size_t)n - cum;  /* column within row r */

    ensure_rows(r);              /* compute needed rows */

    return (seq_t) eul_rows[r][k];  /* value is 0..3, cast to seq_t */
}
