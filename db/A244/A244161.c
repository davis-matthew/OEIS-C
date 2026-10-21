/*
 * A244161  Greedy Catalan Base (A014418) interpreted as base-4 numbers, then
 *          shown in decimal.
 * https://oeis.org/A244161
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 41, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A244161 – Greedy Catalan Base (A014418) interpreted as base‑4
 *
 *   a(0) = 0
 *   a(n) = 4^(k-1) + a(n – C_k)
 *   where k = max{ i | C_i ≤ n } and C_i is the i‑th Catalan number.
 *
 * The algorithm precomputes Catalan numbers as needed, then performs a
 * greedy decomposition of n into distinct Catalan numbers.  For each
 * selected Catalan C_k it adds 4^(k‑1) to the result, which is exactly
 * the base‑4 digit “1” at position k‑1.  All operations use only
 * integer arithmetic allowed by the build environment.  The function
 * works for any seq_t/idx_t combination and is memoised across calls
 * by static storage of the Catalan table. */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* a(0) = 0 */
    if (n == 0) {
        return (seq_t)0;
    }

    /* ---- Catalan numbers table (dynamic, static) ---- */
    enum { MAX_CAT = 80 };                  /* enough for all n <= 10000          */
    static seq_t cat[MAX_CAT];
    static size_t cat_len = 0;              /* number of stored Catalan values   */

    /* Initialise first two Catalan numbers once. */
    if (cat_len == 0) {
        cat_len = 1;
        cat[0] = (seq_t)1;                 /* C0 = 1 */
        if (MAX_CAT > 1) {
            cat[1] = (seq_t)1;             /* C1 = 1 */
            cat_len = 2;
        }
    }

    /* Extend the table until it contains a Catalan number > n. */
    while (cat_len > 0 && cat[cat_len-1] <= (seq_t)n) {
        if (cat_len >= MAX_CAT) {
            break;                         /* safety check */
        }
        size_t k = cat_len - 1;
        /* C_{k+1} = C_k * 2*(2k+1) / (k+2)  */
        seq_t numerator = cat[k] * ((seq_t)2 * (seq_t)(2 * k + 1));
        seq_t next = numerator / ((seq_t)(k + 2));
        cat[cat_len++] = next;
    }

    /* ---- Greedy decomposition of n into Catalan numbers ---- */
    idx_t rem = n;
    seq_t result = (seq_t)0;

    while (rem > 0) {
        /* Find the largest index k with C_k ≤ rem.  Binary search. */
        size_t low  = 0;
        size_t high = cat_len - 1;
        while (low < high) {
            size_t mid = (low + high + 1) >> 1;
            if (cat[mid] <= (seq_t)rem) {
                low = mid;
            } else {
                high = mid - 1;
            }
        }
        size_t k = low;                     /* k ≥ 1 because rem > 0           */

        /* Add the base‑4 digit contribution 4^(k-1). */
        seq_t term = (seq_t)1;
        for (size_t i = 0; i + 1 < k; ++i) {
            term *= (seq_t)4;
        }
        result += term;

        /* Subtract the chosen Catalan number. */
        rem -= (idx_t)cat[k];
    }

    return result;
}
