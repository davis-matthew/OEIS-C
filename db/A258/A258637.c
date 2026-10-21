/*
 * A258637  Number of length 6 1..(n+1) arrays with every leading partial sum
 *          divisible by 2, 3, 5 or 7.
 * https://oeis.org/A258637
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 51, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for OEIS A258637:
 * Number of length‑6 arrays using values 1..(n+1) such that each
 * leading partial sum is divisible by 2, 3, 5 or 7.
 *
 * The calculation uses dynamic programming over residues modulo
 * LCM(2,3,5,7)=210.  For each of the 6 positions we add all
 * possible values and keep only those partial sums whose residue
 * is divisible by at least one of {2,3,5,7}.  The resulting count
 * is returned in the type seq_t.
 */

#include <stdbool.h>

#define LCM 210                 /* lcm of 2,3,5,7 */

static bool allowed[LCM];
static bool allowed_inited = false;

/* initialise allowed[] once */
static void init_allowed(void)
{
    if (allowed_inited) return;
    for (int r = 0; r < LCM; ++r) {
        allowed[r] = (r % 2 == 0) || (r % 3 == 0) ||
                     (r % 5 == 0) || (r % 7 == 0);
    }
    allowed_inited = true;
}

seq_t a(idx_t n)
{
    init_allowed();

    /* dp[r] = number of ways to obtain partial sum ≡ r (mod LCM) */
    seq_t dp[LCM];
    seq_t newdp[LCM];

    for (int i = 0; i < LCM; ++i) dp[i] = 0;
    dp[0] = 1;                 /* empty sum */

    idx_t max_val = n + 1;      /* values from 1 .. n+1 */

    for (idx_t step = 0; step < 6; ++step) {
        for (int i = 0; i < LCM; ++i) newdp[i] = 0;

        for (int r = 0; r < LCM; ++r) {
            seq_t ways = dp[r];
            if (!ways) continue;

            for (idx_t val = 1; val <= max_val; ++val) {
                int val_mod = (int)(val % LCM);         /* small residue */
                int r2 = r + val_mod;
                if (r2 >= LCM) r2 -= LCM;
                if (allowed[r2]) newdp[r2] += ways;
            }
        }
        /* prepare for next position */
        for (int i = 0; i < LCM; ++i) dp[i] = newdp[i];
    }

    /* sum all counts (every residue allowed) */
    seq_t total = 0;
    for (int i = 0; i < LCM; ++i) total += dp[i];
    return total;
}
