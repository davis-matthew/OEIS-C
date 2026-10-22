/*
 * A369792  Triangle read by rows: T(n,k) is the number of steps in
 *          constructing a tree f(n,k) where f(a,b) = f(f(a-1,b), f(a,b-1)),
 *          f(0,b) = f(b-1,b-1), f(a,0) = f(a-1,a-1), f(0,0) = 0; n >= 0, k =
 *          0..n.
 * https://oeis.org/A369792
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabl, nonn
 * Formula:   a(0)=1; for n>0, a(n) is obtained from the triangle T(n,k)
 *            defined by T(0,0)=1, T(n,0)=T(n-1,n-1)+1, T(n,n)=2*T(n,n-1)+2,
 *            T(n,k)=T(n-1,k)+T(n,k-1)+2 (0<k<n). The sequence a(n) is the
 *            triangle read row‑by‑row (triangle read by rows)
 * Range:     u8 n <= 11, u16 n <= 31, u32 n <= 87, u64 n <= 251,
 *            u128 n <= 742, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation stores already‑computed values in a
   static array and expands it only when a larger index is
   requested.  The indexes are determined by incrementally
   keeping track of the current row start and length. */

#include <stdlib.h>   /* malloc, realloc, abort */
#include <stdint.h>   /* size_t */

static seq_t *seq     = NULL;       /* memoised values */
static idx_t seq_len  = 0;          /* number of stored values */

/* current state for the next element to be generated */
static idx_t curr_row   = 0;         /* row number of last element */
static idx_t curr_start = 0;         /* start index of current row */

seq_t a(idx_t n)
{
    /* allocate more space if needed */
    if (n >= seq_len) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        if (!tmp) abort();        /* allocation failed */
        seq = tmp;

        /* initialise the first element if this is the first call */
        if (seq_len == 0) {
            seq[0] = (seq_t)1;    /* T(0,0) */
            seq_len = 1;
            curr_row   = 0;
            curr_start = 0;
        }

        /* generate values from seq_len up to n (inclusive) */
        for (idx_t i = seq_len; i <= n; ++i) {
            /* advance to the row that contains index i */
            while ((curr_start + curr_row) < i) {
                curr_start += curr_row + 1;   /* start of next row */
                curr_row++;                   /* increment row number */
            }

            idx_t col = i - curr_start;      /* column within row */
            seq_t val;

            if (col == 0) {
                /* first column: T(n,0) = T(n-1,n-1) + 1 */
                val = seq[i - 1] + (seq_t)1;
            } else if (col == curr_row) {
                /* last column: T(n,n) = 2*T(n,n-1) + 2 */
                val = seq[i - 1] * (seq_t)2 + (seq_t)2;
            } else {
                /* middle columns: T(n,k) = T(n-1,k) + T(n,k-1) + 2 */
                val = seq[i - 1] + seq[i - curr_row] + (seq_t)2;
            }

            seq[i] = val;
        }

        seq_len = n + 1;  /* update stored length */
    }

    return seq[n];
}
