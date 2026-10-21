/*
 * A272266  The union of squares (A000290) and 10-gonal numbers (A001107).
 * https://oeis.org/A272266
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n‑th smallest positive integer that is either a
 *            square * (A000290) or a 10‑gonal number (A001107), i.e. a(n) =
 *            min{m | m is a * square or m is of the form k*(4k‑3), k≥1},
 *            sorted in increasing order
 * Range:     u8 n <= 21, u16 n <= 381, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static state retained between calls */
    static seq_t *seq = NULL;   /* array of computed sequence values */
    static size_t len = 0;      /* number of values stored in seq   */
    static size_t cap = 0;      /* current capacity of seq array    */
    static idx_t sq_i = 1;      /* next index for squares          */
    static idx_t ten_i = 1;     /* next index for 10‑gonal numbers */

    /* generate terms until we have at least n of them */
    while (len < n) {
        /* next potential values from each sequence */
        seq_t val_sq  = (seq_t)sq_i * (seq_t)sq_i;
        seq_t val_ten = (seq_t)ten_i * (((seq_t)4 * ten_i) - 3);

        seq_t next;
        if (val_sq < val_ten) {
            next = val_sq;
            sq_i++;
        } else if (val_ten < val_sq) {
            next = val_ten;
            ten_i++;
        } else {  /* val_sq == val_ten : duplicate, keep once */
            next = val_sq;
            sq_i++;
            ten_i++;
        }

        /* grow the array if needed */
        if (len == cap) {
            size_t new_cap = (cap == 0) ? 16 : cap * 2;
            /* explicit cast for C++ compatibility */
            seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
            if (!tmp) abort();          /* abort on allocation failure */
            seq = tmp;
            cap = new_cap;
        }
        seq[len++] = next;
    }
    return seq[n - 1];
}
