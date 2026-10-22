/*
 * A340156  Square array read by upward antidiagonals: T(n, k) is the number
 *          of n-ary strings of length k containing 00.
 * https://oeis.org/A340156
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is obtained by reading the square array T(m,k) = number of
 *            m‑ary strings of length k that contain the substring "00" along
 *            upward antidiagonals. For a given index n (n ≥ 2): idx = n - 2
 *            // zero‑based index t = minimal integer ≥ 2 with t*(t-1)/2 > idx
 *            cumPrev = (t-1)*(t-2)/2 p = idx - cumPrev...
 * Range:     u8 n <= 20, u16 n <= 52, u32 n <= 147, u64 n <= 371,
 *            u128 n <= 1067, big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Include the helper that provides pow_seq.  Adjust include path if
 *  the build system places the file elsewhere. */
#include "../utils/pow_seq.h"

/* Static helper that returns T(m,k).  Both m and k are ≥ 2. */
static seq_t
T_value (idx_t mparam, idx_t k)
{
  /* Convert to seq_t once. */
  seq_t m = (seq_t)mparam;
  seq_t total_no_zero = 0;
  seq_t dp0 = 0, dp1 = 0;

  /* Base case: length 1. */
  dp0 = 1;               /* string "0" */
  dp1 = m - 1;           /* strings ending with a non‑zero digit */
  total_no_zero = dp0 + dp1;

  /* Build up to length k. */
  for (idx_t l = 2; l <= k; ++l)
    {
      seq_t new_dp0 = dp1;                    /* append 0 after a non‑zero */
      seq_t new_dp1 = (dp0 + dp1) * (m - 1);  /* append non‑zero after any state */
      dp0 = new_dp0;
      dp1 = new_dp1;
      total_no_zero = dp0 + dp1;
    }

  /* Compute m^k.  k is small (≤ ~200 when n ≤ 10000). */
  seq_t total = pow_seq (m, (int)k);

  /* T(m,k) = m^k – strings with no "00". */
  return total - total_no_zero;
}

/* Public function required by the template.  It is externally linked. */
seq_t a (idx_t n)
{
  /* Offsets for n < 2 are irrelevant; behaviour below  offset does not
   * matter.  For n ≥ 2 compute as described above. */
  if (n < 2)
    return 0;

  idx_t idx = n - 2;                     /* zero‑based index */

  /* Find minimal t ≥ 2 such that t*(t-1)/2 > idx. */
  idx_t t = 2;
  while (t * (t - 1) / 2 <= idx)
    ++t;

  /* Cumulated counts before the current diagonal. */
  idx_t cumPrev = (t - 1) * (t - 2) / 2;

  /* Position inside the diagonal. */
  idx_t p = idx - cumPrev;

  /* Column (k) and row (m) indices. */
  idx_t k  = p + 2;
  idx_t m  = t - p;

  return T_value (m, k);
}
