/*
 * A308759  Sum of the second largest parts of the partitions of n into 4
 *          parts.
 * https://oeis.org/A308759
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A308759 – Sum of the second largest parts of the partitions of n into 4 parts
   Recurrence (valid for n ≥ 16):
      a(n) = a(n‑2)+2·a(n‑3)+2·a(n‑4)
           –2·a(n‑5)-3·a(n‑6)-4·a(n‑7)
           +4·a(n‑9)+3·a(n‑10)+2·a(n‑11)
           –2·a(n‑12)-2·a(n‑13)-a(n‑14)+a(n‑16)
   Initial values (n = 0 … 15):
      0,0,0,0,1,1,3,5,10,13,23,30,46,59,83,103
*/

#include <stdlib.h>

static const seq_t init_vals[16] = {
    0, 0, 0, 0, 1, 1, 3, 5,
    10, 13, 23, 30, 46, 59, 83, 103
};

/* Memoisation array – grows to accommodate requested n */
static seq_t *memo = NULL;       /* dynamic array of computed a(n) */
static idx_t memo_len = 0;       /* number of entries stored */

seq_t a(idx_t n)
{
    /* Ensure the memo array is large enough for the smallest requested n */
    if (memo_len < 16 && n < 16) {
        /* We can answer directly from the initial table */
        return init_vals[n];
    }

    /* Initialise or enlarge the memo array when necessary */
    if (n + 1 > memo_len) {
        idx_t new_len = n + 1;
        seq_t *buf = (seq_t *)realloc(memo, new_len * sizeof(seq_t));
        if (buf == NULL) return 0;          /* unlikely: allocation failure */
        memo = buf;
        memo_len = new_len;

        /* Fill the array.  For indices < 16 we copy from init_vals */
        idx_t start = 0;
        if (memo_len > 16) start = 16;
        for (idx_t i = 0; i < 16 && i < memo_len; ++i)
            memo[i] = init_vals[i];

        /* Compute remaining values using the recurrence */
        for (idx_t i = start; i < memo_len; ++i) {
            seq_t val = 0;
            /* Positive contributions */
            val += memo[i-2];
            val += 2 * memo[i-3];
            val += 2 * memo[i-4];
            val += 4 * memo[i-9];
            val += 3 * memo[i-10];
            val += 2 * memo[i-11];
            val += memo[i-16];
            /* Negative contributions */
            val -= 2 * memo[i-5];
            val -= 3 * memo[i-6];
            val -= 4 * memo[i-7];
            val -= 2 * memo[i-12];
            val -= 2 * memo[i-13];
            val -= memo[i-14];
            memo[i] = val;
        }
    }

    return memo[n];
}
