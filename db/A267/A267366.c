/*
 * A267366  Binary representation of the middle column of the "Rule 126"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A267366
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Computes a(n) by simulating the Rule 126 elementary cellular automaton.
 * a(0)=1, a(1)=11, a(2)=110, a(3)=1101, a(4)=11010, ...
 * The function returns the decimal integer whose decimal digits are the
 * binary digits of the middle column up to generation n.
 */
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* The total width after n steps is 2*n+1; allocate a bit of slack. */
    size_t width = (size_t)n * 2ULL + 3ULL;          /* safe for n=0 */
    size_t center = (size_t)n + 1ULL;                 /* index of the middle cell */

    uint8_t *curr = (uint8_t *)malloc(width);
    uint8_t *next = (uint8_t *)malloc(width);
    if (!curr || !next) {
        free(curr);
        free(next);
        return (seq_t)0;
    }

    memset(curr, 0, width);
    curr[center] = 1;           /* initial single ON cell */
    size_t left = center;       /* current leftmost occupied index */
    size_t right = center;      /* current rightmost occupied index */

    seq_t result = 0;

    for (idx_t step = 0; step <= n; ++step) {
        /* Record the middle cell for this generation. */
        seq_t mid = (seq_t)curr[center];
        result = result * (seq_t)10 + mid;

        if (step == n)
            break;

        /* Compute the next generation for the expanded area. */
        size_t next_left = left - 1;
        size_t next_right = right + 1;

        for (size_t pos = next_left; pos <= next_right; ++pos) {
            uint8_t L = (pos > 0 && pos - 1 >= left && pos - 1 <= right) ?
                        curr[pos - 1] : 0;
            uint8_t C = (pos >= left && pos <= right) ? curr[pos] : 0;
            uint8_t R = (pos + 1 <= right) ? curr[pos + 1] : 0;
            next[pos] = (L == C && C == R) ? 0 : 1;
        }

        /* Prepare for the next iteration. */
        uint8_t *tmp = curr;
        curr = next;
        next = tmp;
        left = next_left;
        right = next_right;
    }

    free(curr);
    free(next);
    return result;
}
