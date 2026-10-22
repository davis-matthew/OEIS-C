/*
 * A344810  Numbers that are the sum of six squares in six or more ways.
 * https://oeis.org/A344810
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 186, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A344810: numbers that are the sum of six positive squares in
   six or more unordered ways.  a(1)=54, a(2)=57, …  The 6 squares are
   allowed to repeat but must all be positive; the 6-tuple is considered
   unordered (a₁≤a₂≤⋯≤a₆).  The algorithm builds a dynamic programming
   table that counts, for each sum s, the number of ways to obtain s with
   exactly k positive squares, saturating the count at 6 (we only care
   whether the count reaches 6).  Once the table is built we collect
   all s whose count for k=6 is at least 6; these are the terms of the
   sequence.  The table is recomputed once on the first call and cached
   for subsequent calls.  The preset maximum `MAX_N` (4 × 10⁴) is ample
   to generate the first 10 000 terms in under 2 s on typical hardware. */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static seq_t   *a_values   = NULL;   /* a(1) at a_values[0] */
static size_t  a_count    = 0;        /* number of terms cached */
static bool    a_ready    = false;   /* cache built? */

/* Pre‑compute the sequence up to a fixed maximum N.  The choice of 4 × 10⁴
   guarantees at least 10 000 terms of the sequence. */
static void build_sequence(void)
{
    if (a_ready) return;
    a_ready = true;

    /* Upper bound for the searched numbers */
    const idx_t MAX_N = 40000;              /* safe for 10 000+ terms */
    const size_t STRIDE = (size_t)MAX_N + 1;/* indices 0 … MAX_N */

    /* dp[k][s] : number of unordered ways to write s as the sum of k
       positive squares (count capped at 6).  Stored as a linear array. */
    unsigned char *dp = (unsigned char *)malloc(7 * STRIDE);
    if (!dp) return;                       /* allocation failed */
    memset(dp, 0, 7 * STRIDE);
    dp[0] = 1;   /* empty representation for k=0, s=0 */

    /* Process each square value v = t², t ≥ 1 */
    for (idx_t t = 1; t * t <= MAX_N; ++t)
    {
        idx_t v = t * t;
        /* For each possible existing number of squares k we can add one
           more square of value v.  Updating in increasing order of s
           allows unlimited usage of v. */
        for (idx_t k = 0; k < 6; ++k)
        {
            unsigned char *src = dp + k * STRIDE;
            unsigned char *dst = dp + (k + 1) * STRIDE;

            size_t limit = (size_t)(MAX_N - v);
            for (size_t s = 0; s <= limit; ++s)
            {
                unsigned char ways = src[s];
                if (!ways) continue;

                size_t idx = s + (size_t)v;
                unsigned char cur = dst[idx];
                unsigned int newv = cur + ways;
                if (newv > 6) newv = 6;    /* saturate at 6 */
                dst[idx] = (unsigned char)newv;
            }
        }
    }

    /* Collect all numbers with at least 6 unordered representations
       using 6 positive squares. */
    seq_t *tmp = (seq_t *)malloc((size_t)MAX_N * sizeof(seq_t));
    if (!tmp) { free(dp); return; }

    size_t cnt = 0;
    unsigned char *dp6 = dp + 6 * STRIDE;   /* k = 6 column */
    for (idx_t s = 1; s <= MAX_N; ++s)
    {
        if (dp6[(size_t)s] >= 6)
        {
            tmp[cnt++] = (seq_t)s;
        }
    }

    free(dp);
    a_values = tmp;
    a_count  = cnt;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined for n=0 */
    build_sequence();
    if (n > a_count) return 0;      /* out of bounds – should not happen */
    return a_values[n - 1];
}
