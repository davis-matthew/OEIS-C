/*
 * A258629  Number of length n 1..(6+1) arrays with every leading partial sum
 *          divisible by 2, 3, 5 or 7.
 * https://oeis.org/A258629
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of length‑n arrays of elements 1..7 such that
 *            every leading partial sum is divisible by at least one of 2, 3,
 *            5 or 7. The implementation uses dynamic programming over
 *            residues modulo lcm(2,3,5,7) = 210. For each step it keeps the
 *            count of ways to reach each residue that already sati...
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 26, u128 n <= 53,
 *            big any n
 * Helpers:   ok_res
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The code relies on the build‑time type definitions:
        typedef ... seq_t;
        typedef ... idx_t;   (uint64_t) */

#include <stdbool.h>
#include "../utils/ok_res.h"

enum { MODS = 210 };

/*  Count the number of valid sequences of length n. */
seq_t a(idx_t n)
{
    if (n == 0) {
        /* Undefined for n < 1; return 0 as a safe default. */
        return (seq_t)0;
    }

    /* Static arrays of size MODS are fixed at compile time.        */
    /* They are zeroed on each call – no persistent state is kept.   */
    static seq_t cur[MODS];
    static seq_t nxt[MODS];
    /* Zero the arrays.  Doing this each call keeps the function
       re‑entrant and avoids reliance on static initialization.   */
    for (int i = 0; i < MODS; ++i) {
        cur[i] = 0;
        nxt[i] = 0;
    }

    /* Initial position – choose the first element.                */
    for (int a0 = 1; a0 <= 7; ++a0) {
        int r = a0 % MODS;
        if (ok_res(r)) {
            cur[r] += (seq_t)1;
        }
    }

    /* Subsequent positions.  For each residue that can occur
       after the previous step, add each allowed element and
       keep only those new residues that satisfy the condition. */
    for (idx_t step = 1; step < n; ++step) {
        /* Clear next array. */
        for (int i = 0; i < MODS; ++i) {
            nxt[i] = 0;
        }

        for (int r = 0; r < MODS; ++r) {
            seq_t ways = cur[r];
            if (ways == 0) {
                continue;
            }
            for (int a = 1; a <= 7; ++a) {
                int r2 = (r + a) % MODS;
                if (ok_res(r2)) {
                    nxt[r2] += ways;
                }
            }
        }

        /* Swap cur and nxt for the next iteration.            */
        for (int i = 0; i < MODS; ++i) {
            cur[i] = nxt[i];
        }
    }

    /* Sum all final residues to obtain the total number of
       sequences of length n. */
    seq_t total = 0;
    for (int r = 0; r < MODS; ++r) {
        total += cur[r];
    }
    return total;
}
