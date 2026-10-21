/*
 * A248392  "Look and say" sequence, but say everything mod 2; starting with
 *          1.
 * https://oeis.org/A248392
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 107, u16 n <= 107, u32 n <= 107, u64 n <= 107,
 *            u128 n <= 107, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The nth digit of the look‑and‑say sequence read modulo 2,
   starting from the digit 1.  For each run of equal digits in the
   current term, output the pair (run length mod 2, digit mod 2). */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n) {
    /* Handle trivial case immediately */
    if (n == 1) return (seq_t)1;

    /* Dynamic buffers for current and next terms */
    size_t cap_cur = 128, cap_next = 128;
    uint8_t *cur = (uint8_t *)malloc(cap_cur);
    uint8_t *next = (uint8_t *)malloc(cap_next);
    if (!cur || !next) return (seq_t)0;  /* allocation failure */

    /* Initialize first term: "1" */
    cur[0] = 1;
    size_t len_cur = 1;
    size_t idx_cur = 0;      /* index within cur */

    idx_t produced = 0;      /* digits already output */

    while (1) {
        /* Output current digit */
        uint8_t digit = cur[idx_cur];
        produced++;
        if (produced == n) {  /* done */
            free(cur);
            free(next);
            return (seq_t)digit;
        }

        /* Advance to next digit in cur */
        idx_cur++;
        if (idx_cur == len_cur) {
            /* Generate next term from cur */
            size_t len_next = 0;
            for (size_t i = 0; i < len_cur; ) {
                uint8_t d = cur[i];
                size_t j = i;
                while (j < len_cur && cur[j] == d) j++;
                uint8_t lenmod = (uint8_t)((j - i) & 1);
                uint8_t digmod = (uint8_t)(d & 1);
                if (len_next + 2 > cap_next) {
                    /* Grow next buffer */
                    size_t new_cap = cap_next * 2;
                    uint8_t *tmp = (uint8_t *)realloc(next, new_cap);
                    if (!tmp) { free(cur); free(next); return (seq_t)0; }
                    next = tmp;
                    cap_next = new_cap;
                }
                next[len_next++] = lenmod;
                next[len_next++] = digmod;
                i = j;
            }

            /* Swap cur and next for the next iteration */
            uint8_t *tmp = cur;
            cur = next;
            next = tmp;
            size_t tmp_cap = cap_cur;
            cap_cur = cap_next;
            cap_next = tmp_cap;

            len_cur = len_next;
            idx_cur = 0;
        }
    }
}
