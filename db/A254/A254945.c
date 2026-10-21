/*
 * A254945  Number of length n 1..(6+1) arrays with every leading partial sum
 *          divisible by 2, 3, 5, 7 or 11.
 * https://oeis.org/A254945
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 26, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A254945: Number of length‑n arrays over {1,2,…,7} whose every leading
   partial sum is divisible by at least one of the primes 2, 3, 5, 7 or 11.
   The algorithm uses dynamic programming over residues modulo the LCM
   2310.  For each residue r we keep the number of sequences that end
   with a partial sum ≡ r (mod 2310).  Transitions add each possible
   next element a (1 ≤ a ≤ 7) and keep only those new residues that
   are divisible by one of the primes.  The final answer is the sum of
   counts for all admissible residues.
   Complexity: O(n · 2310 · 7); fast enough for n up to ~10 000. */

#include <stddef.h>
#include <stdbool.h>

/* --- compile‑time constants ------------------------------------------ */
enum { MOD = 2310 };               /* lcm(2,3,5,7,11) */

/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* ---------- first‑time initialisation of the admissible residues  --- */
    static bool good[MOD];
    static bool initialized = false;
    if (!initialized) {
        for (int r = 0; r < MOD; ++r) {
            good[r] = (r % 2 == 0) || (r % 3 == 0) || (r % 5 == 0) ||
                       (r % 7 == 0) || (r % 11 == 0);
        }
        initialized = true;
    }

    /* ---------- DP arrays: counts for each residue ------------------ */
    seq_t dp_cur[MOD];
    seq_t dp_next[MOD];

    /* initialise */
    for (int i = 0; i < MOD; ++i) {
        dp_cur[i]  = (seq_t)0;
        dp_next[i] = (seq_t)0;
    }
    dp_cur[0] = (seq_t)1;   /* empty sequence, sum 0 */

    /* ---------- iterate over positions  ------------------------------ */
    for (idx_t step = 0; step < n; ++step) {
        /* clear next array */
        for (int i = 0; i < MOD; ++i) {
            dp_next[i] = (seq_t)0;
        }

        /* transition */
        for (int r = 0; r < MOD; ++r) {
            seq_t val = dp_cur[r];
            if (val == (seq_t)0) continue;
            for (int a_val = 1; a_val <= 7; ++a_val) {
                int r2 = r + a_val;
                if (r2 >= MOD) r2 -= MOD;   /* modulo MOD */
                if (good[r2]) {
                    dp_next[r2] += val;
                }
            }
        }

        /* copy next to current for next iteration */
        for (int i = 0; i < MOD; ++i) {
            dp_cur[i] = dp_next[i];
        }
    }

    /* ---------- sum over all admissible residues ------------------- */
    seq_t total = (seq_t)0;
    for (int i = 0; i < MOD; ++i) {
        total += dp_cur[i];
    }

    return total;
}
