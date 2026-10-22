/*
 * A339534  Numbers b > 1 such that the smallest five primes, i.e., 2, 3, 5, 7
 *          and 11 are base-b Wieferich primes.
 * https://oeis.org/A339534
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th smallest integer b > 1 such that for each of the
 *            primes 2, 3, 5, 7, 11 we have b^(p-1) ≡ 1 (mod p²)
 * Range:     u8 none, u16 n <= 1, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    /* static state for one-time initialization of residues */
    static bool initialized = false;
    static seq_t residues[480];   /* maximum possible number of residues */
    static size_t num_residues = 0;
    static seq_t M = 0;           /* modulus = lcm(p_i^2) for p_i = 2,3,5,7,11 */

    if (!initialized) {
        /* Modulus: 4 * 9 * 25 * 49 * 121 = 5,336,100 */
        M = (seq_t)4 * 9 * 25 * 49 * 121;

        for (seq_t r = 1; r < M; ++r) {
            /* Condition for p = 2 : r ≡ 1 (mod 4) */
            if (r % 4 != 1) continue;

            /* Conditions for the other primes */
            if (mod_pow(r, 2,  9) != 1) continue;   /* p = 3 */
            if (mod_pow(r, 4,  25) != 1) continue;  /* p = 5 */
            if (mod_pow(r, 6,  49) != 1) continue;  /* p = 7 */
            if (mod_pow(r, 10, 121) != 1) continue; /* p = 11 */

            if (r == 1) continue;   /* base 1 is not allowed */

            residues[num_residues++] = r;  /* r's are already in ascending order */
        }

        initialized = true;
    }

    /* The nth soluble base is found by binary searching the count function. */
    idx_t target = n;
    seq_t low  = 2;                         /* smallest admissible base */
    seq_t high = (seq_t)((idx_t)n * (idx_t)M + M);
    if (high < low) high = low;

    while (low < high) {
        seq_t mid = low + (high - low) / 2;

        idx_t cnt = 0;
        for (size_t i = 0; i < num_residues; ++i) {
            seq_t r = residues[i];
            if (mid < r) continue;
            cnt += (idx_t)(((mid - r) / M) + 1);
            if (cnt >= target) break; /* early stop */
        }

        if (cnt >= target)
            high = mid;
        else
            low = mid + 1;
    }

    return low;
}
