/*
 * A319092  Triangle read by rows: T(0,0) = 1; T(n,k) = T(n-1, k) + 2*T(n-1,
 *          k-1) + 3*T(n-1, k-2) + 4*T(n-1, k-3) + 5*T(n-1, k-4) + 6*T(n-1,
 *          k-5) for k = 0..5*n; T(n,k)=0 for n or k < 0.
 * https://oeis.org/A319092
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabf, nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Triangle read by rows: T(0,0) = 1;
 *  T(n,k) = T(n-1,k) + 2*T(n-1,k-1) + 3*T(n-1,k-2) +
 *           4*T(n-1,k-3) + 5*T(n-1,k-4) + 6*T(n-1,k-5),
 *  for k = 0..5*n; T(n,k)=0 for n<0 or k<0.
 *
 *  a(n) is the n‑th element of the flattened triangle (row by row).
 *  This implementation pre‑computes rows until the required index
 *  appears, using a convolution of the previous row with the
 *  polynomial 1+2x+3x²+4x³+5x⁴+6x⁵.  The convolution bounds are
 *  carefully checked to avoid out‑of‑range accesses.  The function
 *  is memoised; subsequent calls may request any index in any order.  */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    static bool initialized = false;
    static seq_t *data   = NULL;   /* flattened triangle */
    static size_t data_len = 0;    /* number of elements stored */
    static size_t data_cap = 0;    /* capacity of data array */
    static seq_t *prev   = NULL;   /* coefficients of the previous row */
    static size_t prev_len = 0;    /* length of prev row */
    static idx_t current_row = 0;  /* last computed row index */

    if (!initialized) {
        /* Initial capacity choices are arbitrary but adequate. */
        data_cap = 1024;
        data = (seq_t *)malloc(data_cap * sizeof(seq_t));
        if (!data) return 0;            /* allocation failure */

        prev_len = 1;
        prev = (seq_t *)malloc(prev_len * sizeof(seq_t));
        if (!prev) {
            free(data);
            return 0;
        }

        data[0] = 1;      /* row 0 */
        prev[0] = 1;
        data_len = 1;
        initialized = true;
    }

    /* Compute rows until the flattened array contains index n. */
    while (n >= (idx_t)data_len) {
        idx_t next_row = current_row + 1;
        size_t next_len = 5 * next_row + 1;   /* number of coefficients in new row */

        seq_t *cur = (seq_t *)malloc(next_len * sizeof(seq_t));
        if (!cur) {
            /* Allocation failed: return 0 as a sentinel. */
            return 0;
        }

        /* Polynomial coefficients 1+2x+3x²+4x³+5x⁴+6x⁵. */
        const int coef[6] = {1, 2, 3, 4, 5, 6};

        /* Convolution of prev row with the polynomial.  Bound checks
         * ensure we never read past the end of 'prev'. */
        for (size_t k = 0; k < next_len; ++k) {
            seq_t sum = 0;
            for (int t = 0; t < 6; ++t) {
                if (k >= (size_t)t && (k - t) < prev_len) {
                    sum += (seq_t)coef[t] * prev[k - t];
                }
            }
            cur[k] = sum;
        }

        /* Append the new row to the flattened array, expanding if needed. */
        if (data_len + next_len > data_cap) {
            size_t new_cap = data_cap * 2;
            while (new_cap < data_len + next_len)
                new_cap *= 2;
            seq_t *tmp = (seq_t *)realloc(data, new_cap * sizeof(seq_t));
            if (!tmp) {
                free(cur);
                return 0;
            }
            data = tmp;
            data_cap = new_cap;
        }
        memcpy(data + data_len, cur, next_len * sizeof(seq_t));
        data_len += next_len;

        /* Prepare for the next iteration. */
        free(prev);
        prev = cur;
        prev_len = next_len;
        current_row = next_row;
    }

    /* Index n is now within the pre‑computed range. */
    return data[(size_t)n];
}
