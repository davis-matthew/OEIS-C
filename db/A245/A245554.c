/*
 * A245554  A Rauzy fractal sequence: trajectory of 1 under morphism 1 ->
 *          1,2,1,3; 2 -> 3; 3 -> 1.
 * https://oeis.org/A245554
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 92, u16 n <= 92, u32 n <= 92, u64 n <= 92, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: Start with symbol 1 and iterate the morphism  
   1 → 1,2,1,3; 2 → 3; 3 → 1.  
   a(n) is the nth symbol (1‑indexed).  */
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;               /* index 0 is undefined */
    /* generate the word until it has at least n symbols */
    idx_t curr_len = 1;
    uint8_t *curr = (uint8_t *)malloc(sizeof(uint8_t));
    curr[0] = 1;                                 /* initial word "1" */

    while (curr_len < n) {
        /* count symbols to compute length of next word */
        idx_t cnt1 = 0, cnt2 = 0, cnt3 = 0;
        for (idx_t i = 0; i < curr_len; ++i) {
            uint8_t v = curr[i];
            if (v == 1) ++cnt1;
            else if (v == 2) ++cnt2;
            else ++cnt3;
        }
        idx_t next_len = cnt1 * 4 + cnt2 + cnt3;
        uint8_t *next = (uint8_t *)malloc(sizeof(uint8_t) * next_len);
        idx_t pos = 0;
        for (idx_t i = 0; i < curr_len; ++i) {
            uint8_t v = curr[i];
            if (v == 1) {
                next[pos++] = 1;
                next[pos++] = 2;
                next[pos++] = 1;
                next[pos++] = 3;
            } else if (v == 2) {
                next[pos++] = 3;
            } else { /* v == 3 */
                next[pos++] = 1;
            }
        }
        free(curr);
        curr = next;
        curr_len = next_len;
    }

    uint8_t result = curr[n - 1];   /* nth symbol (0‑based index) */
    free(curr);
    return (seq_t)result;
}
