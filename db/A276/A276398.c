/*
 * A276398  Limit S_oo where S_0 = 0, S_i = S_{i-1} 1^(4^i) S_{i-1} for i > 0.
 * https://oeis.org/A276398
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8127, u16 n <= 8127, u32 n <= 8127, u64 n <= 8127,
 *            u128 n <= 8127, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A276398: infinite sequence defined by
   S_0 = "0"   and   S_i = S_{i-1} 1^(4^i) S_{i-1}  (i > 0).
   The function returns the bit (0 or 1) at index n (0‑based) of the
   limit S_oo.  It uses the recursive construction to locate the bit
   in O(i) time where i is the smallest level with |S_i| > n. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

static uint64_t len[64]      = {0};   /* |S_i|   */
static uint64_t pow4[64]     = {0};   /* 4^i     */
static size_t   levels      = 0;     /* current highest i with |S_i| known */
static bool     init_done   = false;

/* Pre‑compute |S_i| and 4^i up to the first level exceeding n. */
static void ensure_len(uint64_t n)
{
    if (!init_done) {
        init_done = true;
        len[0]  = 1;         /* S_0 = "0" has length 1  */
        pow4[0] = 1;         /* 4^0 = 1                 */
        levels  = 0;
    }
    while (len[levels] <= n) {
        size_t i = levels + 1;
        pow4[i] = pow4[i - 1] * 4ull;
        len[i]  = len[levels] * 2ull + pow4[i];
        levels = i;
    }
}

seq_t a(idx_t n)
{
    uint64_t pos = (uint64_t)n;          /* use unsigned 64‑bit for maths   */
    ensure_len(pos);

    /* Find the smallest i with |S_i| > pos. */
    size_t i = 0;
    while (i <= levels && len[i] <= pos) {
        ++i;
    }

    /* Walk down from level i to level 0. */
    while (i > 0) {
        uint64_t left_len = len[i - 1];
        uint64_t mid_len  = pow4[i];

        if (pos < left_len) {
            /* Still in the left copy of S_{i-1}. */
            --i;
            continue;
        } else if (pos < left_len + mid_len) {
            /* Inside the block of 1s. */
            return (seq_t)1;
        } else {
            /* In the right copy of S_{i-1}. */
            pos -= (left_len + mid_len);
            --i;
            continue;
        }
    }

    /* Reached level 0, which is the single '0'. */
    return (seq_t)0;
}
