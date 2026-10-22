/*
 * A376483  Perfect squares k for which sqrt(k) = (Sum of digits of k) - 2.
 * https://oeis.org/A376483
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, base
 * Formula:   a(n) = the n-th perfect square k satisfying sqrt(k) =
 *            digit_sum(k) – 2
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/digit_sum.h"

static seq_t seq[16];          /* buffer for found squares */
static idx_t size = 0;         /* number of terms found */
static bool init = false;      /* indicates if sequence is pre‑computed */

/* Pre‑compute all solutions up to a safe upper bound.
   The bound is far larger than necessary, but the loop is very short. */
static void compute_seq(void)
{
    const idx_t LIMIT = 1000;  /* upper bound for candidate square roots */
    for (idx_t m = 1; m <= LIMIT; ++m)
    {
        seq_t m_sq = (seq_t)m * (seq_t)m;          /* m² */
        seq_t sum  = digit_sum(m_sq);              /* digit sum of m² */
        if (sum == (seq_t)m + 2U)                  /* condition holds */
        {
            seq[size++] = m_sq;
            if (size >= (idx_t)(sizeof(seq) / sizeof(seq[0])))
                break;                              /* buffer full */
        }
    }
    init = true;
}

seq_t a(idx_t n)
{
    if (!init)
        compute_seq();

    if (n == 0 || n > size)
        return 0;                                 /* out of range */

    return seq[n - 1];                            /* 1‑based indexing */
}
