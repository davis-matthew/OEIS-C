/*
 * A249289  Number of length n+3 0..7 arrays with no four consecutive terms
 *          having the sum of any three elements equal to three times the
 *          fourth.
 * https://oeis.org/A249289
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of length n+3 arrays over alphabet 0..7 with no
 *            four consecutive terms having any three elements' sum equal to
 *            three times the fourth
 * Range:     u8 none, u16 n <= 2, u32 n <= 8, u64 n <= 19, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* encoded transition lists: 512 states × up to 8 next states */
static uint16_t next_states[512][8];
static unsigned char next_counts[512];
static bool initialized = false;

/* Build the transition tables once. */
static void init_transitions(void)
{
    for (int a = 0; a < 8; ++a) {
        for (int b = 0; b < 8; ++b) {
            for (int c = 0; c < 8; ++c) {
                int idx = (a << 6) | (b << 3) | c;
                unsigned char cnt = 0;
                for (int d = 0; d < 8; ++d) {
                    int sumabc = a + b + c;
                    int sumabd = a + b + d;
                    int sumacd = a + c + d;
                    int sumbcd = b + c + d;
                    if (sumabc == 3 * d)            continue; /* forbidden */
                    if (sumabd == 3 * c)            continue;
                    if (sumacd == 3 * b)            continue;
                    if (sumbcd == 3 * a)            continue;
                    uint16_t ns = (b << 6) | (c << 3) | d;
                    next_states[idx][cnt++] = ns;
                }
                next_counts[idx] = cnt;
            }
        }
    }
    initialized = true;
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (!initialized) init_transitions();

    /* Length of the arrays to consider */
    idx_t L = n + 3;   /* n ≥ 1 → L ≥ 4 */

    /* DP tables for current and next positions */
    seq_t dp_cur[512];
    seq_t dp_next[512];

    /* Initial state: all 3‑tuples occur once */
    for (int i = 0; i < 512; ++i) dp_cur[i] = (seq_t)1;

    /* Extend the arrays one position at a time */
    for (idx_t pos = 4; pos <= L; ++pos) {
        for (int i = 0; i < 512; ++i) dp_next[i] = (seq_t)0;

        for (int st = 0; st < 512; ++st) {
            seq_t val = dp_cur[st];
            if (val == (seq_t)0) continue;
            unsigned char cnt = next_counts[st];
            for (int k = 0; k < cnt; ++k) {
                uint16_t ns = next_states[st][k];
                dp_next[ns] += val;
            }
        }

        /* Prepare for next iteration */
        for (int i = 0; i < 512; ++i) dp_cur[i] = dp_next[i];
    }

    /* Sum all ending states to obtain the total count */
    seq_t total = (seq_t)0;
    for (int i = 0; i < 512; ++i) total += dp_cur[i];
    return total;
}
