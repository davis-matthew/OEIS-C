/*
 * A332107  Numbers that are not the sum of seven (7) positive cubes.
 * https://oeis.org/A332107
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   Numbers that are not the sum of seven positive cubes (sequence
 *            A332107). Computation is performed by generating all sums of
 *            exactly seven positive cubes up to the known maximum (2408) and
 *            collecting the numbers that are not representable. The sequence
 *            is finite; the last term a(208) = 2408
 * Range:     u8 n <= 131, u16 n <= 208, u32 n <= 208, u64 n <= 208,
 *            u128 n <= 208, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <string.h>

static seq_t  seq208[208];
static bool   seq_built = false;
static size_t seq_len   = 0;

/* Build the complete sequence once. */
static void build_seq(void)
{
    if (seq_built) return;

    /* Largest term in the sequence. */
    const int MAX_TERM = 2408;
    /* Previous and current reachability arrays. */
    unsigned char prev[MAX_TERM + 1];
    unsigned char cur [MAX_TERM + 1];
    memset(prev, 0, sizeof(prev));
    prev[0] = 1;                        /* base: 0 cubes sum to 0 */

    /* Cubes of positive integers up to the cube root of MAX_TERM. */
    int cube_vals[13];
    for (int i = 1; i <= 13; ++i) cube_vals[i - 1] = i * i * i;   /* 1^3 … 13^3 */

    /* Compute sums of exactly 7 positive cubes. */
    for (int step = 1; step <= 7; ++step) {
        memset(cur, 0, sizeof(cur));
        for (int s = 0; s <= MAX_TERM; ++s) {
            if (prev[s]) {
                for (int j = 0; j < 13; ++j) {
                    int t = s + cube_vals[j];
                    if (t <= MAX_TERM) cur[t] = 1;
                }
            }
        }
        memcpy(prev, cur, sizeof(prev));
    }

    /* Collect numbers that are NOT representable: the sequence terms. */
    size_t idx = 0;
    for (int n = 1; n <= MAX_TERM; ++n) {
        if (!prev[n]) {
            seq208[idx++] = (seq_t)n;
        }
    }
    seq_len   = idx;      /* should be 208 */
    seq_built = true;
}

/* Public function: a(n).  n>=1.  For n>208 the function returns 0. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;     /* offset 1; n must be ≥1 but guard against overflow */
    build_seq();
    if (n <= seq_len) return seq208[(size_t)(n - 1)];
    return 0;                 /* undefined for n>208 in the sequence */
}
