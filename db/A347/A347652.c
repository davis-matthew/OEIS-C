/*
 * A347652  Records in the trajectory of all positive integers in the 3x+1 or
 *          Collatz problem, including the trajectory [1, 4, 2, 1] of 1.
 * https://oeis.org/A347652
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 29, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A347652:
 *   Record highs observed in all Collatz trajectories,
 *   starting from each positive integer in order.
 *   a(1) = 1, a(2) = 4, a(3) = 10, a(4) = 16, ...
 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* States shared among all calls: the list of records,
     * the number of records currently stored, the current
     * global maximum, and the next starting integer to examine.
     * All are static to handle out‑of‑order calls. */
    static seq_t *records = NULL;  /* array of record values   */
    static size_t   rec_cap   = 0;   /* allocated capacity      */
    static size_t   rec_len   = 0;   /* number of stored records*/
    static seq_t   current_max = 0;   /* greatest value seen so far */
    static idx_t   next_start  = 1;  /* next integer to start from */

    if (n == 0) return 0;  /* offset 1 – undefined for n==0 */

    /* ensure the records array can hold one more element */
    #define ENSURE_CAPACITY()                                         \
        do {                                                          \
            if (rec_len == rec_cap) {                                 \
                size_t new_cap = rec_cap ? rec_cap * 2 : 4;            \
                seq_t *tmp = (seq_t *)realloc(records, new_cap *     \
                                                sizeof(seq_t));      \
                records = tmp;                                        \
                rec_cap = new_cap;                                    \
            }                                                         \
        } while (0)

    /* append a new record value and update the maximum */
    #define ADD_RECORD(val)                                           \
        do {                                                          \
            ENSURE_CAPACITY();                                        \
            records[rec_len++] = (val);                               \
            current_max = (val);                                      \
        } while (0)

    /* Generate records until we have at least n of them */
    while (rec_len < (size_t)n)
    {
        seq_t start = (seq_t)next_start++;   /* starting integer (>=1) */
        seq_t val   = start;

        if (start == 1) /* handle the special cycle 1-4-2-1 */
        {
            /* process values until we return to 1 again          */
            do
            {
                if (val > current_max) ADD_RECORD(val);

                /* compute next term */
                if (val & 1U) /* odd */
                    val = val * 3 + 1;
                else          /* even */
                    val = val / 2;
            } while (val != 1); /* stop after the second 1 */
        }
        else
        {
            /* ordinary Collatz trajectory until we hit 1 once */
            while (1)
            {
                if (val > current_max) ADD_RECORD(val);
                if (val == 1) break;
                if (val & 1U) val = val * 3 + 1;
                else          val = val / 2;
            }
        }
    }

    return records[n - 1];
}
