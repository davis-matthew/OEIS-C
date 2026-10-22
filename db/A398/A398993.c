/*
 * A398993  The binomial matrix of the Bell numbers. Square array read by
 *          ascending antidiagonals.
 * https://oeis.org/A398993
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 44, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   binomial, ij_from_index
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/ij_from_index.h"

/* The sequence A398993: a(n) is read by ascending antidiagonals of the
   binomial matrix of the Bell numbers.
   Formula:
     Let (i,j) be the row and column of n in the antidiagonal order.
     Let d = |i-j|,  p = min(i,j),  s = 1 if j≤i else -1.
     a(n) = Σ_{m=0..d} binomial(d,m) · Bell(p+m) · s^{d-m}
   Initial values: a(0)=1, a(1)=2, a(2)=0, a(3)=5, a(4)=1, a(5)=1,
   a(6)=15, a(7)=3, a(8)=1, a(9)=1, a(10)=52, ...
*/

static seq_t *bell = NULL;      /* bell[n] = B(n) */
static idx_t bell_size = 0;

/* Compute Bell numbers up to max_n using the recurrence
 * B(0)=1 , B(n)= Σ_{k=0}^{n-1} binomial(n-1,k)*B(k) */
static void ensure_bell(idx_t max_n)
{
    if (bell == NULL) {
        bell = (seq_t *)malloc(sizeof(seq_t));
        if (!bell) exit(EXIT_FAILURE);
        bell[0] = 1;
        bell_size = 1;
    }
    if (bell_size <= max_n) {
        seq_t *tmp = (seq_t *)realloc(bell, (max_n + 1) * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        bell = tmp;
        for (idx_t n = bell_size; n <= max_n; ++n) {
            seq_t sum = 0;
            for (idx_t k = 0; k < n; ++k) {
                sum += binomial(n - 1, k) * bell[k];
            }
            bell[n] = sum;
        }
        bell_size = max_n + 1;
    }
}

seq_t a(idx_t n)
{
    idx_t i, j;
    ij_from_index(n, &i, &j);

    idx_t max_b = (i > j) ? i : j;
    ensure_bell(max_b);

    idx_t d = (i > j) ? i - j : j - i;
    idx_t p = (i < j) ? i : j;

    seq_t res = 0;
    if (j <= i) {                         /* s = +1 */
        for (idx_t m = 0; m <= d; ++m) {
            res += binomial(d, m) * bell[p + m];
        }
    } else {                               /* s = –1 : alternate signs */
        for (idx_t m = 0; m <= d; ++m) {
            seq_t term = binomial(d, m) * bell[p + m];
            if ((d - m) & 1)    /* odd exponent → negative */
                res -= term;
            else
                res += term;
        }
    }
    return res;
}
