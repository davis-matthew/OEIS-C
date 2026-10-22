/*
 * A335475  Numbers k such that the k-th composition in standard order
 *          (A066099) matches the pattern (1,2,2).
 * https://oeis.org/A335475
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 38, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation of A335475:
 * a(n) = the n-th integer k such that the k-th composition in
 * the standard order (graded reverse-lexicographic, A066099)
 * matches the pattern (1,2,2).  The function enumerates
 * compositions one by one, tests the pattern with an
 * O(length) routine, and stops when the requested term is found.
 */
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* offset starts at 1 */
    idx_t found = 0;
    idx_t k = 1;
    seq_t result = 0;

    /* Main loop: iterate over k until we have collected the nth match */
    while (found < n)
    {
        /* ---- compute the k-th composition in standard order ---- */
        /* parts are stored after reversal, i.e. in the correct order */
        int parts[128] = {0};
        int len = 0;
        int max_part = 0;

        idx_t tmp = k;
        int pos = 1;                  /* position counter (1‑based) */
        int prev = 0;                 /* previous 1‑bit position */

        while (tmp)                   /* scan bits of k, LSB first */
        {
            if (tmp & 1) {
                int diff = pos - prev;
                parts[len++] = diff;
                if (diff > max_part) max_part = diff;
                prev = pos;
            }
            ++pos;
            tmp >>= 1;
        }

        /* reverse the array of differences to get the composition */
        for (int i = 0; i < len / 2; ++i) {
            int t = parts[i];
            parts[i] = parts[len - 1 - i];
            parts[len - 1 - i] = t;
        }

        /* ---- test for pattern (1,2,2) ---- */
        /* count_total[v] = total occurrences of value v in this composition */
        int count_total[128] = {0};
        for (int i = 0; i < len; ++i) {
            int v = parts[i];
            count_total[v] += 1;
        }

        /* count_right[v] = number of occurrences of v strictly after current index */
        int count_right[128];
        for (int i = 0; i < 128; ++i) count_right[i] = count_total[i];

        unsigned int min_so_far = 0xFFFFFFFFU;   /* minimal value before current index */
        bool match = false;

        for (int i = 0; i < len; ++i) {
            int v = parts[i];
            /* decrement to exclude current position */
            count_right[v]--;
            if (count_right[v] > 0 && min_so_far < (unsigned int)v) {
                match = true;
                break;
            }
            if ((unsigned int)v < min_so_far) min_so_far = v;
        }

        if (match) {
            ++found;
            if (found == n) {
                result = (seq_t)k;
                break;
            }
        }
        ++k;
    }
    return result;
}
