/*
 * A395345  Triangle read by rows: T(n, k) = k * T(n, k+1) + T(n-1, k-1) -
 *          (n-k-1) * T(n-1, k) with T(n, 0) = 0^n, T(n, k) = 0 for k < 0 or k
 *          > n.
 * https://oeis.org/A395345
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 45, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle read by rows:
   T(n, k) = k * T(n, k+1) + T(n-1, k-1) - (n-k-1) * T(n-1, k)
   with T(n,0) = 0^n,   T(n,k)=0 for k<0 or k>n.
   The sequence a(n) is obtained by concatenating the rows of T(n,k)
   (row 0 first, then row 1, etc.).  Therefore a(0)=T(0,0)=1,
   a(1)=T(1,0), a(2)=T(1,1), a(3)=T(2,0), ...
   The implementation builds rows up to the one that contains the
   required index and returns the corresponding element. */
seq_t a(idx_t n)
{
    /* Handle the trivial first element directly.  */
    if (n == 0)
        return (seq_t)1;

    /* Allocate two small fixed-size buffers that are big enough
       for all rows that are needed for n <= 10000
       (max row index ≈ 141).  */
    seq_t row0[200] = {0};
    seq_t row1[200] = {0};

    seq_t *prev = row0;   /* previous row (row r-1) */
    seq_t *curr = row1;   /* current row (row r) */

    /* Row 0 is known: T(0,0) = 1.  */
    prev[0] = (seq_t)1;
    idx_t cumulative = 1;  /* number of elements processed so far */

    /* If the requested index lies before row 0, we would have
       returned already.  Now start building rows from r = 1 upwards. */
    for (idx_t r = 1; ; ++r) {
        idx_t len = r + 1;                     /* length of row r    */
        idx_t new_cumulative = cumulative + len;

        /* If the required index is inside this row, compute
           the row and return the element at position
           offset = n - cumulative.             */
        if (n < new_cumulative) {
            idx_t offset = n - cumulative;

            /* Build row r from prev (row r-1).  */
            seq_t next = 0; /* T(r, k+1) for the next iteration   */
            for (idx_t k = r; ; --k) {
                seq_t term1 = (seq_t)k * next;
                seq_t term2 = (k >= 1) ? prev[k - 1] : (seq_t)0;
                seq_t term3 = (k < r) ? ( ((seq_t)(r - k - 1)) * prev[k] ) : (seq_t)0;
                curr[k] = term1 + term2 - term3;
                next = curr[k];
                if (k == 0)
                    break;
            }

            return curr[offset];
        }

        /* Otherwise the entire row is before the desired index.
           Build the row (throw away the values later).    */
        seq_t next = 0;
        for (idx_t k = r; ; --k) {
            seq_t term1 = (seq_t)k * next;
            seq_t term2 = (k >= 1) ? prev[k - 1] : (seq_t)0;
            seq_t term3 = (k < r) ? ( ((seq_t)(r - k - 1)) * prev[k] ) : (seq_t)0;
            curr[k] = term1 + term2 - term3;
            next = curr[k];
            if (k == 0)
                break;
        }

        /* Advance to next iteration: make curr the new prev.   */
        seq_t *tmp  = prev;
        prev = curr;
        curr = tmp;
        cumulative = new_cumulative;
    }
}
