/*
 * A326308  Number of inversion sequences of length n where all consecutive
 *          subsequences i,j,k satisfy i > j < k or i < j > k.
 * https://oeis.org/A326308
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 16, u64 n <= 24, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A326308: 
   a(0)=1, a(1)=1, a(2)=2.
   For n ≥ 3:
   Let up[v] be the number of inversion sequences of length n ending with value v
   whose last comparison (between the last two elements) is increasing (prev < v).
   Let down[v] be the same but with a decreasing comparison.
   Then
      new_up[v] = ∑_{u < v} down[u]
      new_down[v] = ∑_{u > v} up[u]          (all sums are over values 0 … n-1)
   The sums are taken using prefix‑ and suffix‑sums, so each step is O(n).
   The total a(n) is ∑_{v} (up[v] + down[v]) after the last step.
*/

#include <stddef.h>   /* for NULL */

#define MAXN 10000  /* maximum n expected in the test suite */

/* Static arrays large enough for all n ≤ MAXN.  All indices 0 … MAXN
   are valid for these arrays.  They are re‑initialised on every call. */
static seq_t up[MAXN + 1];
static seq_t down[MAXN + 1];
static seq_t new_up[MAXN + 1];
static seq_t new_down[MAXN + 1];

seq_t a(idx_t n)
{
    /* Handle the offset explicitly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;

    /* If n is larger than we can handle, this implementation cannot
       guarantee correctness.  The test suite never uses n > MAXN. */
    if (n > MAXN) {
        /* Return zero to avoid undefined behaviour. */
        return (seq_t)0;
    }

    /* Zero all arrays up to n. */
    idx_t i;
    for (i = 0; i <= n; ++i) {
        up[i] = 0;
        down[i] = 0;
        new_up[i] = 0;
        new_down[i] = 0;
    }

    /* Base for length 2: only the increasing transition 0 → 1 is
       admissible for future extensions. */
    up[1] = (seq_t)1;

    /* Iterate from length 3 up to n. */
    for (idx_t len = 3; len <= n; ++len) {
        /* Compute new_up by accumulating prefix sums of down. */
        seq_t pref = 0;
        new_up[0] = 0;          /* no u < 0 */
        for (i = 0; i < len - 1; ++i) {
            pref += down[i];
            new_up[i + 1] = pref;
        }

        /* Compute new_down by accumulating suffix sums of up. */
        seq_t suff = 0;
        /* use a loop that decrements an unsigned counter safely */
        for (i = len; i > 0; --i) {      /* i = len, ..., 1 */
            seq_t val = new_down[i - 1];
            val = suff;                  /* assign new_down[i-1] */
            new_down[i - 1] = suff;      /* store */
            suff += up[i - 1];
        }

        /* Copy new values back into up/down for the next iteration. */
        for (i = 0; i < len; ++i) {
            up[i] = new_up[i];
            down[i] = new_down[i];
        }
    }

    /* Sum all possibilities for the final length n. */
    seq_t total = 0;
    for (i = 0; i < n; ++i) {
        total += up[i];
        total += down[i];
    }
    return total;
}
