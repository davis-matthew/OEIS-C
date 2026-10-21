/*
 * A286226  Number of compositions (ordered partitions) of n into parts with
 *          an odd number of prime divisors (counted with multiplicity).
 * https://oeis.org/A286226
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of compositions of n into parts whose number of
 *            prime divisors (counted with multiplicity) is odd
 * Range:     u8 n <= 16, u16 n <= 30, u32 n <= 57, u64 n <= 112,
 *            u128 n <= 222, big any n
 * Helpers:   bigomega, ensure_capacity_char
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/bigomega.h"   /* defines seq_t bigomega(idx_t) */
#include "../utils/ensure_capacity_char.h"

/* generic capacity grow for seq_t arrays */
static void ensure_capacity_seq(seq_t **arr, size_t *cap, size_t needed)
{
    if (*cap >= needed) return;
    size_t new_cap = *cap ? *cap : 1;
    while (new_cap < needed) new_cap <<= 1;
    *arr = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
    *cap = new_cap;
}

seq_t a(idx_t n)
{
    static seq_t *dp  = NULL;         /* dp[i] = a(i) */
    static size_t dp_cap = 0;
    static idx_t dp_max = (idx_t)-1;   /* -1 = uninitialised */

    static unsigned char *odd = NULL;  /* odd[k] = 1 if bigomega(k) odd */
    static size_t odd_cap = 0;

    /* initialise for the first call */
    if (dp_max == (idx_t)-1) {
        ensure_capacity_seq(&dp, &dp_cap, 1);
        dp[0] = 1;           /* a(0) = 1 */

        ensure_capacity_char(&odd, &odd_cap, 1);
        odd[0] = 0;          /* placeholder, unused */

        dp_max = 0;
    }

    /* extend precomputation if needed */
    if (n > dp_max) {
        /* enlarge arrays to hold values up to n */
        ensure_capacity_seq(&dp, &dp_cap, (size_t)n + 1);
        ensure_capacity_char(&odd, &odd_cap, (size_t)n + 1);

        /* compute parity of bigomega for new indices */
        for (idx_t k = dp_max + 1; k <= n; ++k) {
            odd[k] = ((bigomega(k) & 1) != 0) ? 1 : 0;
        }

        /* dynamic programming for new dp values */
        for (idx_t i = dp_max + 1; i <= n; ++i) {
            seq_t sum = 0;
            for (idx_t k = 1; k <= i; ++k) {
                if (odd[k]) sum += dp[i - k];
            }
            dp[i] = sum;
        }

        dp_max = n;
    }

    return dp[n];
}
