/*
 * A353932  Irregular triangle read by rows where row k lists the run-sums of
 *          the k-th composition in standard order.
 * https://oeis.org/A353932
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A353932: run‑sums of compositions in standard order
   (graded reverse‑lexicographic). The function generates the
   compositions via recursion and yields the n-th run‑sum. */
#include <stdint.h>
#include <stdbool.h>

static idx_t g_cur_idx;
static idx_t g_target_n;
static seq_t g_target_val;
static unsigned char g_comp[256];   /* maximum composition length needed */

static bool gen_composites(int rem, int len)
{
    if (rem == 0) {                     /* reached a full composition */
        unsigned run_val = g_comp[0];
        unsigned run_len = 1;
        for (int i = 1; i < len; ++i) {
            unsigned v = g_comp[i];
            if (v == run_val) {
                ++run_len;
            } else {
                /* output previous run sum */
                g_cur_idx++;
                if (g_cur_idx == g_target_n) {
                    g_target_val = (seq_t)run_val * (seq_t)run_len;
                    return true;
                }
                run_val = v;
                run_len = 1;
            }
        }
        /* final run */
        g_cur_idx++;
        if (g_cur_idx == g_target_n) {
            g_target_val = (seq_t)run_val * (seq_t)run_len;
            return true;
        }
        return false;
    }
    /* choose next part in descending order -> reverse‑lexicographic order */
    for (int f = rem; f >= 1; --f) {
        g_comp[len] = (unsigned char)f;
        if (gen_composites(rem - f, len + 1))
            return true;
    }
    return false;
}

seq_t a(idx_t n)
{
    g_target_n = n;
    g_cur_idx = 0;
    /* enumerate compositions of increasing total sum until the target is reached */
    for (int total = 1; ; ++total) {
        if (gen_composites(total, 0))
            return g_target_val;
    }
}
