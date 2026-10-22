/*
 * A399721  The map of the "Lucas-Collatz" function: if n is Lucas-odd
 *          (A399306), then a(n)=2*n; if n is Lucas-even (A399305), then a(n)
 *          is the integer whose minimal Lucas representation (A130310)
 *          results from removing the final 0 of A130310(n).
 * https://oeis.org/A399721
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Implementation of OEIS A399721 (Lucas‑Collatz map).

   For an integer n ≥ 1 let L_k denote the Lucas numbers
   L_0 = 2, L_1 = 1, L_k = L_{k-1} + L_{k-2}.  
   The minimal Lucas representation of n is the unique set of
   distinct indices i (no two consecutive) such that
   n = Σ L_i, with the smallest possible number of terms.
   The representation is encoded as a binary string whose i‑th
   bit (from the least significant) is 1 iff index i is used.

   A(n) is defined by:
      – If the representation ends in a 1 (i.e. L_0 is used),
        A(n) = 2·n.
      – Otherwise, drop the trailing 0 from the representation.
        The resulting binary string must be a valid minimal Lucas
        representation; if it is, A(n) is the integer represented
        by that string.  If it is not, A(n) = 2·n.

   The function below computes the representation of a number using
   a feasibility DP and then applies the above rule.
   It is fully compliant with the required interface.
*/

#include <stdbool.h>
#include <stddef.h>

#define MAX_LUCAS 256   /* enough for all values fitting in seq_t */

static seq_t lucas[MAX_LUCAS];
static size_t lucas_len = 0;

/* Build Lucas numbers up to at least LIMIT (inclusive if possible) */
static void ensure_lucas_upto(seq_t limit)
{
    if (lucas_len == 0) {
        lucas[0] = (seq_t)2;
        lucas[1] = (seq_t)1;
        lucas_len = 2;
    }
    while (lucas_len < MAX_LUCAS) {
        seq_t next = lucas[lucas_len-1] + lucas[lucas_len-2];
        if (next < lucas[lucas_len-1])   /* overflow guard */
            break;
        lucas[lucas_len++] = next;
        if (next > limit)
            break;
    }
    /* guarantee at least one Lucas number strictly greater than limit */
    if (lucas_len < MAX_LUCAS && lucas[lucas_len-1] <= limit) {
        seq_t next = lucas[lucas_len-1] + lucas[lucas_len-2];
        if (next >= lucas[lucas_len-1]) /* still okay */
            lucas[lucas_len++] = next;
    }
}

/* Compute the index set of the minimal Lucas representation of VAL.
   The indices are stored in OUT (descending order) and the count
   is written to COUNT_OUT.  Returns the number of indices.  */
static size_t lucas_indices(seq_t val, unsigned *out, size_t *count_out)
{
    ensure_lucas_upto(val);

    /* Find the largest index whose Lucas number ≤ val */
    size_t max_index = lucas_len - 1;
    while (max_index > 0 && lucas[max_index] > val)
        --max_index;

    /* Maximal representable sums using indices ≤ i, no consecutive   */
    seq_t max_sum[MAX_LUCAS];
    max_sum[0] = lucas[0];
    if (max_index >= 1)
        max_sum[1] = lucas[1] > max_sum[0] ? lucas[1] : max_sum[0];
    for (size_t i = 2; i <= max_index; ++i) {
        seq_t use_i = lucas[i] + max_sum[i-2];
        seq_t skip_i = max_sum[i-1];
        max_sum[i] = use_i > skip_i ? use_i : skip_i;
    }

    /* Build the representation using feasibility test   */
    seq_t remaining = val;
    size_t idx = 0;
    long long cur = (long long)max_index;
    while (remaining > 0 && cur >= 0) {
        bool found = false;
        for (long long i = cur; i >= 0; --i) {
            if (remaining >= lucas[i]) {
                seq_t new_rem = remaining - lucas[i];
                seq_t max_possible = (i >= 2) ? max_sum[i-2] : (seq_t)0;
                if (new_rem <= max_possible) {
                    out[idx++] = (unsigned)i;
                    remaining = new_rem;
                    cur = i - 2;
                    found = true;
                    break;
                }
            }
        }
        if (!found)
            break;  /* should never happen for valid inputs */
    }
    *count_out = idx;
    return idx;
}

/* Main sequence function ---------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;           /* offset 1; not used */

    seq_t n_val = (seq_t)n;

    unsigned idx_n[256];
    size_t cnt_n = 0;
    lucas_indices(n_val, idx_n, &cnt_n);

    bool used_L0 = false;
    seq_t m_val = (seq_t)0;
    for (size_t i = 0; i < cnt_n; ++i) {
        unsigned idx = idx_n[i];
        if (idx == 0)
            used_L0 = true;
        else
            m_val += lucas[idx-1];
    }

    if (used_L0)
        return (seq_t)2 * n_val;   /* Lucas‑odd case */

    /* Check whether dropping the final 0 yields a valid minimal representation */
    unsigned idx_m[256];
    size_t cnt_m = 0;
    lucas_indices(m_val, idx_m, &cnt_m);

    bool equal = (cnt_m == cnt_n);
    if (equal) {
        for (size_t i = 0; i < cnt_n; ++i) {
            if (idx_m[i] != idx_n[i] - 1) {
                equal = false;
                break;
            }
        }
    }

    if (equal)
        return m_val;               /* Lucas‑even case */
    else
        return (seq_t)2 * n_val;    /* fails test → treat as Lucas‑odd */
}
