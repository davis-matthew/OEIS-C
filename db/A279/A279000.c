/*
 * A279000  Numbers of the form (11*h+j)*11^k-1 for h,k in N and j in
 *          {1,3,4,5,9}.
 * https://oeis.org/A279000
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   seq_ok
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A279000 – Numbers of the form (11*h + j)*11^k – 1
   with h,k ∈ ℕ and j ∈ {1,3,4,5,9}.
   The function returns the n‑th term (offset 1) computed by a
   straightforward search over successive integers. */

/* ---------- Dependencies ---------- */
#include <stdbool.h>
#include <stdint.h>
#include "../utils/seq_ok.h"

/* ---------- Sequence function ---------- */
seq_t a(idx_t n)
{
    /* n >= 1 (offset 1).  Return the n-th term. */
    idx_t count = 0;
    seq_t val = 0U;

    for (;;)
    {
        if (seq_ok(val))
        {
            ++count;
            if (count == n)
                return val;
        }
        ++val;
    }
}
