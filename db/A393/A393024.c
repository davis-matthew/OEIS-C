/*
 * A393024  Numbers that can be written as s_1^x_1 + ... + s_t^x_t, with 0 <
 *          s_1 < ... < s_t and x_1 > ... > x_t > 1 for some t > 0, with the
 *          restriction that if s_1 = 1 then x_1 is the least possible.
 * https://oeis.org/A393024
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Enumerate all numbers that can be written as a sum of distinct
 *            powers s_i^x_i with 0 < s_1 < ... < s_t and x_1 > ... > x_t > 1
 *            (and with the restriction about s_1 = 1). All integers >1190 are
 *            representable (conjectured). Compute all representable numbers
 *            up to LIMIT=1190. For n larger than the am...
 * Range:     u8 none, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define LIMIT      1190
#define MAX_EXP    60          /* sufficiently large for all needed exponents */

/* Global state (cached) */
static bool rep[LIMIT + 1] = {0};
static seq_t a_list[LIMIT + 1];   /* sorted list of representable numbers up to LIMIT */
static size_t arr_count = 0;      /* number of representable numbers <= LIMIT */
static size_t missing_count = 0;  /* number of missing integers in 1..LIMIT */
static bool built = false;

/* Recursive enumeration of representable numbers.
   cur       = current sum already built
   last_base = last base used (ensures increasing bases)
   max_exp   = maximum exponent allowed for the next term
*/
static void dfs(seq_t cur, int last_base, int max_exp)
{
    for (int base = last_base + 1; ; ++base) {
        /* base^2 grows too fast; stop when base*base > LIMIT */
        seq_t base_sq = (seq_t)base * (seq_t)base;
        if (base_sq > LIMIT) break;

        seq_t val = 1;
        for (int e = 1; e <= max_exp; ++e) {
            /* stop if multiplying would exceed LIMIT */
            if (val > LIMIT / (seq_t)base) {
                val = LIMIT + 1; /* mark overflow */
                break;
            }
            val *= (seq_t)base;
            if (val > LIMIT) break;

            if (e >= 2) {
                seq_t new_val = cur + val;
                if (new_val > LIMIT) continue;
                rep[(idx_t)new_val] = true;
                dfs(new_val, base, e - 1);
            }
        }
    }
}

/* Build the cache of representable numbers up to LIMIT */
static void build(void)
{
    /* Reset the bitmap */
    for (int i = 0; i <= LIMIT; ++i) rep[i] = false;

    /* Enumerate all possibilities starting from 0 */
    dfs(0, 0, MAX_EXP);

    /* Extract the sorted representable numbers */
    arr_count = 0;
    for (int i = 1; i <= LIMIT; ++i) {
        if (rep[i]) a_list[arr_count++] = (seq_t)i;
    }
    missing_count = LIMIT - arr_count;
}

/* Sequence function.  Thread‑safe as long as builds are atomic. */
seq_t a(idx_t n)
{
    if (!built) { build(); built = true; }

    if (n <= arr_count) {
        /* n is 1‑based index into the cached list */
        return a_list[n - 1];
    }
    /* All larger numbers appear, so just add the offset of missing
       entries below LIMIT. */
    return (seq_t)(n + missing_count);
}
