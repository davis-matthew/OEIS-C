/*
 * A350205  a(n) is the smallest positive integer which can be represented as
 *          the sum of distinct nonzero tetrahedral numbers in exactly n ways,
 *          or 0 if no such integer exists.
 * https://oeis.org/A350205
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A350205: a(n) is the smallest positive integer that can be
   expressed as a sum of distinct non‑zero tetrahedral numbers in exactly
   n ways, or 0 if no such integer exists.  We compute the whole table
   once by a dynamic‑programming bit‑set style subset‑sum on the
   tetrahedral numbers up to an upper bound. */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

#define MAX_N_LIMIT  ((size_t)50000)   /* maximal n we precompute    */
#define MAX_SUM_LIMIT ((size_t)200000) /* bound on tetrahedron sums  */

static seq_t min_for_n[MAX_N_LIMIT + 1];          /* minimal sum for each count */
static uint32_t dp[MAX_SUM_LIMIT + 1];            /* subset‑sum counts        */
static idx_t tetra[200];                         /* storage for tetrahedral numbers */
static size_t tlen = 0;                          /* actual number of tetrahedra */

static bool table_ready = false;

seq_t a(idx_t n)
{
    if (n == 0 || n > MAX_N_LIMIT)
        return (seq_t)0;

    if (!table_ready) {
        /* ----- generate tetrahedral numbers T_k = k(k+1)(k+2)/6 <= LIMIT ----- */
        for (idx_t k = 1; ; ++k) {
            idx_t t = k * (k + 1) * (k + 2) / 6;
            if (t > MAX_SUM_LIMIT)
                break;
            tetra[tlen++] = t;
        }

        /* ----- dynamic programming: count subset sums ----- */
        dp[0] = 1;
        for (size_t i = 0; i < tlen; ++i) {
            size_t t = (size_t)tetra[i];
            for (size_t s = MAX_SUM_LIMIT; ; --s) {
                if (s < t)
                    break;
                uint32_t val = dp[s] + dp[s - t];
                if (val > MAX_N_LIMIT + 1)   /* cap to avoid overflow */
                    val = (uint32_t)(MAX_N_LIMIT + 1);
                dp[s] = val;
                if (s == 0)   /* prevent unsigned wraparound */
                    break;
            }
        }

        /* ----- record minimal sum for each representation count ----- */
        for (size_t s = 1; s <= MAX_SUM_LIMIT; ++s) {
            uint32_t c = dp[s];
            if (c >= 1 && c <= MAX_N_LIMIT) {
                if (min_for_n[c] == (seq_t)0 || s < (size_t)min_for_n[c])
                    min_for_n[c] = (seq_t)s;
            }
        }

        table_ready = true;
    }

    return min_for_n[(size_t)n];
}
