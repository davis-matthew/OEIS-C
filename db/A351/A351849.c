/*
 * A351849  Irregular triangle read by rows, in which row n lists the
 *          computation of the tag system T_C(3,2) with alphabet {1, 2, 3},
 *          deletion number 2, and production rules 1 -> 23, 2 -> 1, 3 -> 111,
 *          when started from the word encoding n.
 * https://oeis.org/A351849
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the n‑th word produced in the irregular triangle of the
 *            tag system T_C(3,2) with alphabet {1,2,3}, removal number 2, and
 *            production rules 1 → 23, 2 → 1, 3 → 111. The function simulates
 *            the tag system starting from each row of n ones, collecting the
 *            resulting words in order until the n‑th...
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 96, u64 n <= 120, u128 n <= 851,
 *            big any n
 * Helpers:   push_digit
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/push_digit.h"

seq_t a(idx_t n)
{
    /* n is a 1‑based index in the flattened sequence. */
    size_t produced = 0;

    for (idx_t start = 1; ; ++start) {
        /* Allocate a word buffer and initialise it with `start` ones. */
        uint8_t *word = NULL;
        size_t cap = 0, len = 0;

        for (idx_t i = 0; i < start; ++i)
            push_digit(&word, &len, &cap, 1);

        size_t head = 0;   /* index of the first symbol currently available */

        while (1) {
            /* Convert the current word (from head to len-1) to a seq_t value. */
            seq_t val = 0;
            for (size_t i = head; i < len; ++i)
                val = val * (seq_t)10 + (seq_t)word[i];

            ++produced;
            if (produced == (size_t)n) {
                free(word);
                return val;
            }

            /* Halting condition: the word is exactly "1". */
            if (len - head == 1 && word[head] == 1)
                break;

            /* At this point there must be at least two symbols to delete. */
            if (len - head < 2)
                break;

            uint8_t first = word[head];
            head += 2;   /* delete the first two symbols */

            /* Append the production of the removed symbol. */
            if (first == 1) {
                push_digit(&word, &len, &cap, 2);
                push_digit(&word, &len, &cap, 3);
            } else if (first == 2) {
                push_digit(&word, &len, &cap, 1);
            } else {   /* first == 3 */
                push_digit(&word, &len, &cap, 1);
                push_digit(&word, &len, &cap, 1);
                push_digit(&word, &len, &cap, 1);
            }
        }

        free(word);
    }
}
