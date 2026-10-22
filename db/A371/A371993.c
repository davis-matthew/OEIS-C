/*
 * A371993  The smallest number k that has a factorization k = f1*f2*...*fr
 *          where the digits of {k, f1, f2, ..., fr} together contain all, and
 *          only, the digits 0..n, where n<=9.
 * https://oeis.org/A371993
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full, changed
 * Formula:   The smallest number k that has a factorization k = f1*f2*...*fr
 *            where the digits of {k, f1, f2, ..., fr} together contain all,
 *            and only, the digits 0..n (n ≤ 9). The sequence is indexed from
 *            1. The function a(n) is computed by brute‑force search and
 *            cached for all 1 ≤ n ≤ 9
 * Range:     u8 none, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   compute_a_for_n, digit_mask_calc, search_factorization
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/compute_a_for_n.h"
#include "../utils/digit_mask_calc.h"
#include "../utils/search_factorization.h"

/* Public sequence function a(n) with external linkage. */
seq_t a(idx_t n)
{
    /* Cache values for 1 ≤ n ≤ 9.  The sequence is undefined for n>9. */
    static seq_t cache[10];
    static bool ready = false;
    if (!ready) {
        for (idx_t i = 1; i <= 9; ++i)
            cache[i] = compute_a_for_n((unsigned)i);
        ready = true;
    }
    if (n >= 1 && n <= 9)
        return cache[(idx_t)n];
    return 0;   /* undefined for n > 9 */
}
