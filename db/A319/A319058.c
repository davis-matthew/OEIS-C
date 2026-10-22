/*
 * A319058  Number of relatively prime aperiodic factorizations of n into
 *          factors > 1.
 * https://oeis.org/A319058
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of relatively prime aperiodic factorizations of n
   into factors > 1 (OEIS A319058). */

/* Use external gcd utility */
#include <stddef.h>
#include "../utils/gcd.h"

/* Integer GCD for small non‑negative values. */
static inline int int_gcd(int a, int b)
{
    while (b) {
        int t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Recursive enumeration of unordered factorizations. */
static void dfs(idx_t rem, idx_t start, idx_t *cur, size_t depth, seq_t *count)
{
    if (rem == 1) {
        /* Check conditions for the current factorization. */
        seq_t all_gcd = (seq_t)0;
        for (size_t i = 0; i < depth; ++i) {
            seq_t v = (seq_t)cur[i];
            all_gcd = (all_gcd == (seq_t)0) ? v : gcd(all_gcd, v);
        }
        if (all_gcd != (seq_t)1) return;

        int mult_gcd = 0;
        for (size_t i = 0; i < depth; ) {
            idx_t val = cur[i];
            int freq = 1;
            ++i;
            while (i < depth && cur[i] == val) {
                ++freq;
                ++i;
            }
            mult_gcd = (mult_gcd == 0) ? freq : int_gcd(mult_gcd, freq);
        }
        if (mult_gcd != 1) return;

        (*count)++;
        return;
    }

    /* Factor equal to the remaining value (single‑factor case). */
    if (rem >= start) {
        cur[depth] = rem;
        dfs(1, rem, cur, depth + 1, count);
    }

    /* Proper divisors in non‑decreasing order. */
    for (idx_t d = start; d <= rem / d; ++d) {
        if (rem % d == 0) {
            cur[depth] = d;
            dfs(rem / d, d, cur, depth + 1, count);
        }
    }
}

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)0;
    seq_t result = (seq_t)0;
    idx_t cur[64];   /* sufficient depth for n up to at least 2^64. */
    dfs(n, 2, cur, 0, &result);
    return result;
}
