/*
 * A305632  Expansion of Product_{r = 1 or not a perfect power} 1/(1 +
 *          (-x)^r).
 * https://oeis.org/A305632
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in ∏_{r=1 or r not a perfect power}
 *            1/(1+(-x)^r)
 * Range:     u8 n <= 29, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses an O(n²) DP over the admissible parts
 * (all r ≤ n except perfect powers >1).  Two arrays store the
 * number of ways to reach each sum with even/odd parity of the
 * accumulated sign.  For odd r the sign never changes, for
 * even r it toggles every time the part is used.  The final
 * coefficient a(n) is even[n] – odd[n], which is guaranteed
 * non‑negative.  All intermediate work is done with seq_t
 * operations, which are integer or arbitrary‑precision.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n) {
    /* zero‑index offset: a(0)=1 */
    if (n == 0) return (seq_t)1;

    const size_t N = (size_t)n;          /* safe for positive n up to 2⁶⁴-1 */

    /* ---- Allocate DP arrays ------------------------------------ */
    seq_t *even = (seq_t *)malloc((N + 1) * sizeof(seq_t));
    seq_t *odd  = (seq_t *)malloc((N + 1) * sizeof(seq_t));
    if (!even || !odd) {
        free(even); free(odd);
        return (seq_t)0;
    }

    /* initialise to zero */
    for (size_t i = 0; i <= N; ++i) {
        even[i] = (seq_t)0;
        odd[i]  = (seq_t)0;
    }
    even[0] = (seq_t)1;   /* empty partition has even sign */

    /* ---- Precompute perfect‑power flags up to N ---------------- */
    bool *isPP = (bool *)calloc(N + 1, sizeof(bool));
    if (!isPP) {
        free(even); free(odd);
        return (seq_t)0;
    }

    for (uint64_t base = 2; base * base <= (uint64_t)N; ++base) {
        uint64_t val = base * base;
        while (val <= (uint64_t)N) {
            isPP[(size_t)val] = true;
            if (val > ((uint64_t)~0ULL) / base) break;  /* avoid overflow */
            val *= base;
        }
    }

    /* ---- Main DP: process admissible parts --------------------- */
    for (size_t r = 1; r <= N; ++r) {
        if (r > 1 && isPP[r]) continue;   /* skip perfect powers >1 */

        if ((r & 1ULL) == 0) { /* even r: sign toggles with each use */
            for (size_t s = r; s <= N; ++s) {
                seq_t add_even = odd[s - r];   /* using r once flips parity */
                seq_t add_odd  = even[s - r];
                even[s] += add_even;
                odd[s]  += add_odd;
            }
        } else { /* odd r: sign never changes */
            for (size_t s = r; s <= N; ++s) {
                even[s] += even[s - r];
                odd[s]  += odd[s - r];
            }
        }
    }

    /* ---- Final coefficient ------------------------------------- */
    seq_t result;
    if (even[N] >= odd[N]) {
        result = even[N] - odd[N];
    } else {
        /* theoretically impossible but safe fallback */
        result = (seq_t)0;
    }

    /* ---- Clean up --------------------------------------------- */
    free(even); free(odd); free(isPP);
    return result;
}
