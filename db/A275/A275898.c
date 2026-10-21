/*
 * A275898  Read the infinite chessboard underlying A065188 by successive
 *          antidiagonals and record when the queens are encountered. Here the
 *          rows and columns are indexed starting at 1 (as in A065188).
 * https://oeis.org/A275898
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = index of the n‑th queen when the infinite chessboard of *
 *            A065188 is read by successive antidiagonals and queens are
 *            placed * greedily so that no two attack each other
 * Range:     u8 n <= 11, u16 n <= 171, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <string.h>

/* Maximum diagonal sum needed for n <= 10000.
 * With the observed growth, s never exceeds 20000. */
#define MAX_S      20000          /* maximum possible antidiagonal sum */
#define MAX_COORD  (MAX_S + 1)    /* maximum row / column index */
#define DIFF_OFF   (MAX_S)        /* offset to make difference non‑negative */
#define DIFF_SIZE  (2 * MAX_S + 1)
#define SUM_SIZE   (MAX_S + 1)

static uint8_t row_used[MAX_COORD];
static uint8_t col_used[MAX_COORD];
static uint8_t diff_used[DIFF_SIZE];
static uint8_t sum_used[SUM_SIZE];

/* Computes a(n) for 1 <= n. */
seq_t a(idx_t n)
{
    /* clear state for this call */
    memset(row_used, 0, sizeof(row_used));
    memset(col_used, 0, sizeof(col_used));
    memset(diff_used, 0, sizeof(diff_used));
    memset(sum_used, 0, sizeof(sum_used));

    size_t count = 0;        /* number of queens found so far */

    for (int s = 2; count < n; ++s)          /* diagonal sum s = i + j */
    {
        for (int i = 1; i < s; ++i)          /* iterate cells on diagonal s */
        {
            int j = s - i;                    /* coordinates (i, j) */

            /* test whether a queen can be placed at (i, j) */
            if (row_used[i] || col_used[j] ||
                diff_used[i - j + DIFF_OFF] ||
                sum_used[s])
                continue;                     /* square is attacked or occupied */

            /* place the queen */
            row_used[i] = col_used[j] =
            diff_used[i - j + DIFF_OFF] = sum_used[s] = 1;

            ++count;
            if (count == n)                   /* the n-th queen found */
            {
                /* index = ((s-2)(s-1))/2 + i */
                seq_t tri = ((seq_t)(s - 2) * (seq_t)(s - 1)) / 2;
                return tri + (seq_t)i;
            }
        }
    }

    /* Should never reach here for valid n >= 1 */
    return 0;
}
