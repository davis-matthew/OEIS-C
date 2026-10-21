/*
 * A268626  Number of nX6 0..2 arrays with some element plus some horizontally
 *          or vertically adjacent neighbor totalling two no more than once.
 * https://oeis.org/A268626
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×6 arrays with elements 0..2 such that some
 *            element plus some horizontally or vertically adjacent neighbor
 *            totals 2 no more than once. Recurrence (n>15): a(n) = 60*a(n‑1)
 *            –1482*a(n‑2) +20016*a(n‑3) –167163*a(n‑4) +924894*a(n‑5)
 *            –3537110*a(n‑6) +9593166*a(n‑7) –18693789*a(n‑8) +2...
 * Range:     u8 none, u16 n <= 2, u32 n <= 6, u64 n <= 14, u128 n <= 29,
 *            big any n
 * Helpers:   parse_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/parse_uint.h"

/* Initial values as strings.  Stored to avoid large integer literals. */
static const char * const init_str[15] = {
    "336",
    "8586",
    "187224",
    "3819654",
    "74914554",
    "1430057208",
    "26758514760",
    "493042858032",
    "8974328440044",
    "161737670836314",
    "2891047134413940",
    "51321708535136676",
    "905714517532142694",
    "15902932976899644264",
    "277998507169027800234"
};

static seq_t init[15];
static bool init_done = false;
static void init_values(void)
{
    for (idx_t i = 0; i < 15; ++i) {
        init[i] = parse_uint(init_str[i]);
    }
    init_done = true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* not defined by OEIS, but safe guard */
    if (!init_done) init_values();

    if (n <= 15) return init[n - 1];

    /* Circular buffer of the last 15 values */
    seq_t cur[15];
    for (idx_t i = 0; i < 15; ++i) cur[i] = init[i];

    for (idx_t i = 16; i <= n; ++i) {
        /* indices for a(i-k) */
        seq_t a1  = cur[(i - 2) % 15];
        seq_t a2  = cur[(i - 3) % 15];
        seq_t a3  = cur[(i - 4) % 15];
        seq_t a4  = cur[(i - 5) % 15];
        seq_t a5  = cur[(i - 6) % 15];
        seq_t a6  = cur[(i - 7) % 15];
        seq_t a7  = cur[(i - 8) % 15];
        seq_t a8  = cur[(i - 9) % 15];
        seq_t a9  = cur[(i -10) % 15];
        seq_t a10 = cur[(i -11) % 15];
        seq_t a11 = cur[(i -12) % 15];
        seq_t a12 = cur[(i -13) % 15];
        seq_t a13 = cur[(i -14) % 15];
        seq_t a14 = cur[(i -15) % 15];

        /* Compute positive and negative contributions separately to avoid
           underflow in unsigned arithmetic. */
        seq_t pos = 0, neg = 0;

        pos += (seq_t)60  * a1;
        pos += (seq_t)20016 * a3;
        pos += (seq_t)924894 * a5;
        pos += (seq_t)9593166 * a7;
        pos += (seq_t)26218062 * a9;
        pos += (seq_t)18139608 * a11;
        pos += (seq_t)2220864 * a13;

        neg += (seq_t)1482   * a2;
        neg += (seq_t)167163 * a4;
        neg += (seq_t)3537110 * a6;
        neg += (seq_t)18693789 * a8;
        neg += (seq_t)26188161 * a10;
        neg += (seq_t)8262712 * a12;
        neg += (seq_t)266256 * a14;

        seq_t val = pos - neg;

        cur[(i - 1) % 15] = val;
    }

    return cur[(n - 1) % 15];
}
