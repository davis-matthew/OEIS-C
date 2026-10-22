/*
 * A332108  Numbers that are not the sum of eight (8) positive cubes.
 * https://oeis.org/A332108
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = numbers that are NOT expressible as a sum of eight
 *            positive cubes
 * Range:     u8 n <= 114, u16 n <= 142, u32 n <= 142, u64 n <= 142,
 *            u128 n <= 142, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Complete all necessary headers for the build environment */
#include <stdbool.h>
#include <stddef.h>

/* Constants for the computation */
static const int LIMIT = 620;                /* last known non‑representable number */
static const int cube[9] = {                 /* cubes of 1..8 */
    0, 1, 8, 27, 64, 125, 216, 343, 512
};

/* Tables used by the single–pass computation */
static bool represent[621];                 /* true if number <= LIMIT is representable */
static seq_t seq_arr[150];                  /* store the sequence; 150 > 142 needed terms */
static idx_t seq_len = 0;
static bool seq_ready = false;

/* Recursive helper: generate all sorted 8‑tuples of numbers 1..8
   and mark the sum in represent[] if it does not exceed LIMIT. */
static void gen_rec(int depth, int sum, int start)
{
    if (depth == 8) {
        if (sum <= LIMIT)
            represent[sum] = true;
        return;
    }
    for (int v = start; v <= 8; ++v) {
        int ns = sum + cube[v];
        if (ns > LIMIT)
            continue;
        gen_rec(depth + 1, ns, v);
    }
}

/* One‑time initialization: find all representable numbers up to LIMIT and
   fill seq_arr with the non‑representable ones in increasing order. */
static void compute_sequence(void)
{
    if (seq_ready)
        return;

    /* clear representation table */
    for (int i = 0; i <= LIMIT; ++i)
        represent[i] = false;

    /* generate all sorted combinations of eight cubes */
    gen_rec(0, 0, 1);

    /* collect the numbers that are NOT representable */
    seq_len = 0;
    for (int i = 1; i <= LIMIT; ++i) {
        if (!represent[i]) {
            seq_arr[seq_len++] = (seq_t)i;
            if (seq_len >= 150)          /* safety */
                break;
        }
    }
    seq_ready = true;
}

/* External function required by the test harness */
seq_t a(idx_t n)
{
    compute_sequence();
    if (n == 0 || (idx_t)(n - 1) >= seq_len)
        return (seq_t)0;                 /* out of bounds – not used in tests */
    return seq_arr[n - 1];
}
