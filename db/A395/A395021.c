/*
 * A395021  Triangle read by rows: T(n, k) = (n! / k!) * [x^n] (
 *          (2*sinh(x/2))^k * (exp(x/2))^(k mod 2) ).
 * https://oeis.org/A395021
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A395021 – T(n,k) triangle read by rows.
   Compute the nth term (with offset 1).
   Recurrence:
      T(n,1)=T(n,n)=1
      k even, n odd  → T(n,k)=0
      k even, n even → T(n,k)=2*T(n,k-1)/k      (current row)
      k odd,  n even → T(n,k)=T(n-1,k)*(k+1)/2
      k odd,  n odd  → T(n,k)=T(n-1,k)*(k+1)/2 + T(n-1,k-2)*2/(k-1)
   This function builds the triangle rows up to the required row and
   returns the requested entry.  No caching across calls is used.  The
   algorithm runs in O(r²) with r ≈ √(2n). */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* determine the row r so that r*(r+1)/2 >= n */
    idx_t r = 1;
    idx_t sum = 1;                 /* 1*2/2 */
    while (sum < n) {
        ++r;
        sum += r;                  /* r*(r+1)/2 */
    }
    /* position k within row r (1‑based) */
    idx_t k = n - (r - 1) * r / 2;

    /* trivial case */
    if (r == 1) {
        return (seq_t)1;
    }

    /* allocate two working rows of maximum size r+1 (index 1..r) */
    size_t maxsize = (size_t)(r + 1);
    seq_t *row0 = (seq_t *)malloc(maxsize * sizeof(seq_t));
    seq_t *row1 = (seq_t *)malloc(maxsize * sizeof(seq_t));
    if (!row0 || !row1) { /* allocation failed – return 0 */
        if (row0) free(row0);
        if (row1) free(row1);
        return (seq_t)0;
    }

    seq_t *prev = row0;   /* will hold previous row (starts with row 1) */
    seq_t *cur  = row1;   /* temporary current row */
    prev[1] = 1;          /* row 1 */

    /* build rows 2..r sequentially */
    for (idx_t i = 2; i <= r; ++i) {
        cur[1] = cur[i] = 1;   /* boundaries */

        /* compute interior entries */
        for (idx_t j = 2; j <= i - 1; ++j) {
            if ((j & 1) == 0) {                 /* j even */
                if ((i & 1) == 1) {             /* i odd → 0 */
                    cur[j] = 0;
                } else {                        /* i even → 2*T(i,j-1)/j */
                    cur[j] = (cur[j - 1] * 2) / j;
                }
            } else {                             /* j odd */
                if ((i & 1) == 0) {             /* i even */
                    cur[j] = prev[j] * ((j + 1) / 2);
                } else {                        /* i odd */
                    seq_t t1 = prev[j] * ((j + 1) / 2);
                    seq_t t2 = 0;
                    if (j >= 3) {
                        t2 = prev[j - 2] * 2 / (j - 1);
                    }
                    cur[j] = t1 + t2;
                }
            }
        }

        /* swap rows for next iteration */
        seq_t *tmp = prev;
        prev = cur;
        cur = tmp;
    }

    /* result is in prev[k] (row r) */
    seq_t result = prev[k];

    /* clean up */
    free(row0);
    free(row1);

    return result;
}
