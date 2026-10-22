/*
 * A328314  Lexicographically earliest infinite sequence such that a(i) = a(j)
 *          => A025586(i) = A025586(j) for all i, j.
 * https://oeis.org/A328314
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 4134, u32 n <= 9205, u64 n <= 9205,
 *            u128 n <= 9205, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 *  Formula: a(n) = label of the maximum value reached in the 3x+1
 *  trajectory of n.  Labels are assigned in the order of first
 *  appearance of each distinct maximum (restricted‑growth transform
 *  of A025586).  a(1) = 1, a(2) = 2, a(3) = 3, ...
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

/* --- internal helper structure for the peak → label map --- */
struct map_entry { seq_t peak; idx_t label; };

/* --- main function --- */
seq_t a(idx_t n)
{
    /* static storage for previous results */
    static seq_t   *peak_arr  = NULL;   /* peak of A025586(1..len) */
    static idx_t   *label_arr = NULL;   /* lexicographic label of each index */
    static size_t  peak_cap  = 0, label_cap = 0;
    static size_t  peak_len  = 0, label_len = 0;

    /* static map from peak value to assigned label */
    static struct map_entry *map = NULL;
    static size_t           map_cap = 0, map_len = 0;

    if (n == 0)   /* offset is 1; this case should not be used */
        return (seq_t)0;

    /* compute all values up to n if necessary */
    for (idx_t i = (idx_t)peak_len + 1; i <= n; ++i)
    {
        /* --- compute A025586(i): the maximum in the 3x+1 trajectory of i --- */
        seq_t cur = (seq_t)i;
        seq_t max_val = cur;
        while (cur != (seq_t)1)
        {
            if ((cur & (seq_t)1) == (seq_t)0)
                cur >>= 1;
            else
                cur = cur * (seq_t)3 + (seq_t)1;
            if (cur > max_val)
                max_val = cur;
        }

        /* --- find or create a label for this peak value --- */
        idx_t lbl = 0;
        for (size_t k = 0; k < map_len; ++k)
            if (map[k].peak == max_val)
            {
                lbl = map[k].label;
                break;
            }

        if (lbl == 0)    /* new peak value encountered */
        {
            lbl = (idx_t)(map_len + 1);

            if (map_len == map_cap)
            {
                size_t new_cap = map_cap ? map_cap << 1 : 4;
                struct map_entry *tmp =
                    (struct map_entry *)realloc(map, new_cap * sizeof(*map));
                map = tmp;
                map_cap = new_cap;
            }
            map[map_len].peak  = max_val;
            map[map_len].label = lbl;
            ++map_len;
        }

        /* --- store peak and label for this index --- */
        if (peak_len == peak_cap)
        {
            size_t new_cap = peak_cap ? peak_cap << 1 : 16;
            seq_t *tmp = (seq_t *)realloc(peak_arr, new_cap * sizeof(*peak_arr));
            peak_arr = tmp;
            peak_cap = new_cap;
        }
        peak_arr[peak_len] = max_val;
        ++peak_len;

        if (label_len == label_cap)
        {
            size_t new_cap = label_cap ? label_cap << 1 : 16;
            idx_t *tmp = (idx_t *)realloc(label_arr, new_cap * sizeof(*label_arr));
            label_arr = tmp;
            label_cap = new_cap;
        }
        label_arr[label_len] = lbl;
        ++label_len;
    }

    return (seq_t)label_arr[(size_t)n - 1];
}
