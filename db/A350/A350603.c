/*
 * A350603  Irregular triangle read by rows: row n lists the elements of the
 *          set S_n in increasing order, where S_0 = {0}, and S_n is obtained
 *          by applying the operations x -> x+1 and x -> 2*x to S_{n-1}.
 * https://oeis.org/A350603
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 229, u16 n <= 8604, u32 n <= 8604, u64 n <= 8604,
 *            u128 n <= 8604, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes sequence A350603 by iteratively constructing the sets S_n
   and selecting the element at the given index.  */
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

/* Assumes seq_t and idx_t are defined by the build (see requirements). */

static seq_t fib[128];          /* Fibonacci numbers, fib[0]=0, fib[1]=1 */
static size_t fib_ready = 0;    /* # elements actually computed in fib[] */

static seq_t *rows[64] = {0};   /* pointers to the sorted arrays S_n   */
static size_t row_sizes[64] = {0};
static size_t rows_computed = 0;/* number of rows already computed */

/* Ensure Fibonacci numbers are available up to index up_to.  */
static void ensure_fib(unsigned up_to) {
    if (fib_ready == 0) {
        fib[0] = 0;
        fib[1] = 1;
        fib_ready = 2;
    }
    while (fib_ready <= up_to) {
        fib[fib_ready] = fib[fib_ready - 1] + fib[fib_ready - 2];
        fib_ready++;
    }
}

/* Build all needed rows up to r_target inclusive.  */
static void compute_rows_upto(size_t r_target) {
    /* Make sure we have enough Fibonacci numbers for size calculations. */
    ensure_fib(r_target + 4);

    for (size_t r = rows_computed; r <= r_target; ++r) {
        if (r == 0) {
            rows[0] = (seq_t *)malloc(sizeof(seq_t));
            rows[0][0] = 0;
            row_sizes[0] = 1;
        } else {
            size_t prev_sz = row_sizes[r - 1];
            size_t target_sz = (size_t)fib[r + 2];   /* |S_r| = Fib(r+2) */
            seq_t *curr = (seq_t *)malloc(target_sz * sizeof(seq_t));

            size_t i = 0, j = 0, k = 0;
            seq_t last = 0;
            int first = 1;   /* true for first element written */

            /* Merge the two sorted lists:  prev[i]+1  and  prev[j]*2  */
            while (i < prev_sz && j < prev_sz && k < target_sz) {
                seq_t v1 = rows[r - 1][i] + 1;
                seq_t v2 = rows[r - 1][j] * 2;
                seq_t take;
                if (v1 < v2) { take = v1; i++; }
                else if (v2 < v1) { take = v2; j++; }
                else { take = v1; i++; j++; }

                if (first || take != last) {
                    curr[k++] = take;
                    last = take;
                    first = 0;
                }
            }

            while (i < prev_sz && k < target_sz) {
                seq_t take = rows[r - 1][i] + 1;
                if (first || take != last) {
                    curr[k++] = take;
                    last = take;
                    first = 0;
                }
                i++;
            }

            while (j < prev_sz && k < target_sz) {
                seq_t take = rows[r - 1][j] * 2;
                if (first || take != last) {
                    curr[k++] = take;
                    last = take;
                    first = 0;
                }
                j++;
            }

            rows[r] = curr;
            row_sizes[r] = target_sz;
        }
    }
    rows_computed = r_target + 1;
}

/* Main sequence function.  */
seq_t a(idx_t n) {
    /* Determine the row r containing index n */
    size_t r = 0;
    size_t cum = 0;   /* cumulative count up to (but not including) current r */

    while (1) {
        ensure_fib(r + 4);
        size_t fsize = (size_t)fib[r + 2];     /* |S_r| */
        size_t next_cum = cum + fsize;
        if ((size_t)n < next_cum) break;
        cum = next_cum;
        r++;
    }

    size_t offset = (size_t)n - cum;   /* position within row r */

    /* Ensure needed rows are computed */
    compute_rows_upto(r);

    return rows[r][offset];
}
