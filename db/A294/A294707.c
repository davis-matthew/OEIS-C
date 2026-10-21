/*
 * A294707  Number of permutations of [n] avoiding {4321, 3142, 1234}.
 * https://oeis.org/A294707
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   avoid_pattern, generate
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Computation of OEIS A294707:
   number of permutations of [n] avoiding the patterns {4321, 3142, 1234}.
   For n ≥ 10 the result is 0 by Erdos–Szekeres.
   For n ≤ 9 we enumerate all permutations and count those that avoid
   all three patterns. Results are memoised for subsequent calls.
*/

#include <stdbool.h>
#include "../utils/avoid_pattern.h"
#include "../utils/generate.h"

/* maximum n for non‑zero values (+1 for zero index) */
#define MAX_PRECAL 10

/* memoised results (a(0) = 1, a(1) = 1, …, a(9)) */
static seq_t precomp[MAX_PRECAL] = {1};
static bool ready[MAX_PRECAL] = {true};

/* ------------------------------------------------------------------ */

/* ------------------------------------------------------------------ */
/* recursive generation of all permutations of 1..n, counting the     */

/* ------------------------------------------------------------------ */
/* public function requested by the build system                       */
seq_t a(idx_t n)
{
    if (n >= (idx_t)MAX_PRECAL)            /* n ≥ 10 → answer is zero   */
        return 0;

    int ni = (int)n;                       /* safe to cast, max 9       */
    if (!ready[ni]) {
        int perm[MAX_PRECAL];
        bool used[MAX_PRECAL];
        for (int i = 1; i <= ni; ++i) used[i] = false;
        seq_t cnt = 0;
        generate(ni, 0, perm, used, &cnt);
        precomp[ni] = cnt;
        ready[ni] = true;
    }
    return precomp[ni];
}
