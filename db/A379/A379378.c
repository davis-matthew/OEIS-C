/*
 * A379378  The n-th term of the n-th forward differences of partition numbers
 *          A000041.
 * https://oeis.org/A379378
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{i=0..n} (-1)^{n-i} * C(n,i) * p(n+i)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 28, u64 n <= 56, u128 n <= 114,
 *            big any n
 * Helpers:   grow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/grow_seq.h"

static seq_t *part   = NULL;   /* array of partition numbers p(0..)          */
static size_t part_len = 0;    /* number of values already known           */
static size_t part_cap = 0;    /* allocated capacity of part                */

static void ensure_partitions(idx_t up_to)
{
    /* nothing to do if already computed                         */
    if (up_to < part_len)
        return;

    /* increase capacity if necessary (grow_seq handles construction) */
    if (part_cap < (size_t)up_to + 1)
        grow_seq(&part, &part_cap, (size_t)up_to + 1);

    /* initialize first entry if this is the first call          */
    if (part_len == 0) {
        part[0] = (seq_t)1;      /* p(0) = 1 */
        part_len = 1;
    }

    /* compute missing partition numbers using the pentagonal‑number theorem */
    for (idx_t m = part_len; m <= up_to; ++m) {
        seq_t sum = 0;
        for (idx_t k = 1; ; ++k) {
            idx_t pent1 = k * (3*k - 1) / 2;
            if (pent1 > m)
                break;

            seq_t term = part[(size_t)(m - pent1)];
            if (k & 1)
                sum += term;
            else
                sum -= term;

            idx_t pent2 = k * (3*k + 1) / 2;
            if (pent2 <= m) {
                term = part[(size_t)(m - pent2)];
                if (k & 1)
                    sum += term;
                else
                    sum -= term;
            }
        }
        part[(size_t)m] = sum;
    }
    part_len = (size_t)up_to + 1;
}

seq_t a(idx_t n)
{
    /* partition numbers up to p(n+n) are required          */
    idx_t need = n * 2;
    ensure_partitions(need);

    seq_t binom  = 1;   /* C(n,0)                              */
    seq_t result = 0;   /* running sum of the formula         */

    for (idx_t i = 0; i <= n; ++i) {
        seq_t term = binom * part[(size_t)(n + i)];
        if (((n - i) & 1ull) == 0)
            result += term;
        else
            result -= term;

        /* update binomial coefficient for the next term:
           C(n,i+1) = C(n,i) * (n-i)/(i+1)                 */
        if (i < n) {
            seq_t nxt = binom * (seq_t)(n - i);
            nxt /= (seq_t)(i + 1);
            binom = nxt;
        }
    }

    return result;
}
