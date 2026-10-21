/*
 * A253667  Square array read by ascending antidiagonals, T(n, k) =
 *          k!*[x^k](exp(-x) *sum(j=0..n, C(n,j)*x^j)), n>=0, k>=0.
 * https://oeis.org/A253667
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   not stated
 * Range:     i8 n <= 65, i16 n <= 65, i32 n <= 65, i64 n <= 65, i128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A253667
   Square array T(n,k) defined by
      T(0,0) = 1
      T(i,0) = 1  (i > 0)
      T(0,k) = (-1)^k  (k > 0)
      T(i,k) = T(i-1,k) + k * T(i-1,k-1)   (i>0, k>0)

   The sequence a(n) is the diagonal read of this array:
      let s be the unique integer with
             s*(s+1)/2 <= n < (s+1)*(s+2)/2
      let o = n - s*(s+1)/2   (0 <= o <= s)
   then
      a(n) = T(s-o, o)        */

#include <stdlib.h>
#include <stdint.h>

/* linear storage for the square array T(0..N,0..N) */
static seq_t *grid = NULL;     /* NULL until first allocation */
static idx_t  grid_size = 0;   /* largest index for which grid is valid */

/* build a new square array of dimension (new_size+1)^2 */
static void extend_grid(idx_t new_size)
{
    /* free old array if any */
    if (grid != NULL)
        free(grid);

    /* allocate new array */
    idx_t dim = new_size + 1;
    grid = (seq_t *)malloc((size_t)dim * (size_t)dim * sizeof(seq_t));
    if (grid == NULL)
        exit(1);  /* allocation failed */

    /* fill the array using the recurrence */
    for (idx_t i = 0; i <= new_size; ++i) {
        for (idx_t j = 0; j <= new_size; ++j) {
            if (i == 0) {                /* T(0,j) = (-1)^j */
                grid[i * dim + j] = (j & 1) ? (seq_t)-1 : (seq_t)1;
            } else if (j == 0) {          /* T(i,0) = 1 for i>0 */
                grid[i * dim + j] = (seq_t)1;
            } else {                      /* recurrence */
                grid[i * dim + j] =
                    grid[(i - 1) * dim + j]          /* T(i-1,j) */
                  + (seq_t)j * grid[(i - 1) * dim + (j - 1)];
            }
        }
    }

    grid_size = new_size;
}

/* main sequence value function */
seq_t a(idx_t n)
{
    /* find s such that s*(s+1)/2 <= n < (s+1)*(s+2)/2 */
    idx_t s = 0;
    while ((s + 1) * (s + 2) / 2 <= n)
        ++s;

    /* offset along the antidiagonal */
    idx_t t = s * (s + 1) / 2;
    idx_t o = n - t;            /* 0 <= o <= s */

    /* indices in the square array */
    idx_t n_prime = s - o;
    idx_t k_prime = o;

    /* ensure grid is large enough */
    if (grid == NULL || grid_size < s)
        extend_grid(s);

    idx_t dim = grid_size + 1;  /* current dimension */
    return grid[n_prime * dim + k_prime];
}
