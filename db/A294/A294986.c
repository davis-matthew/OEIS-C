/*
 * A294986  Number of compositions (ordered partitions) of 1 into exactly 7n+1
 *          powers of 1/(n+1).
 * https://oeis.org/A294986
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of compositions of 1 into exactly 7n+1 powers of
 *            1/(n+1)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 1, u64 n <= 3, u128 n <= 7,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation enumerates all valid carry chains that represent such
   compositions and counts the corresponding ordered arrangements.  */

#include "../utils/binom.h"

/* Compute the number of compositions for a given carry chain. */
static void process_chain(const idx_t *chain, idx_t len, seq_t *acc, idx_t B, seq_t total_terms)
{
    seq_t ways = 1;
    seq_t remaining = total_terms;
    idx_t prev = 0;

    for (idx_t i = 0; i < len; ++i)
    {
        seq_t a_k = (seq_t)B * chain[i] - prev;
        if (a_k > remaining) return;            /* invalid cascade */
        ways *= binom((idx_t)remaining, (idx_t)a_k);
        remaining -= a_k;
        prev = chain[i];
    }

    /* after processing all carries, remaining should be 0 (a_m = 1 - prev = 0) */
    if (remaining != 0) return;
    *acc += ways;
}

/* Depth‑first search over admissible carry sequences. */
static void dfs(idx_t prev, idx_t rem, idx_t pos, idx_t *chain, seq_t *accum,
                idx_t B, seq_t total_terms)
{
    if (rem == 0)
    {
        /* The final carry must be 1 (otherwise a_m would be negative). */
        if (prev == 1)
            process_chain(chain, pos, accum, B, total_terms);
        return;
    }

    /* Minimum allowed carry at this position. */
    idx_t min_c = (prev == 0) ? (idx_t)1 : (prev + B - 1) / B;

    for (idx_t c = min_c; c <= rem; ++c)
    {
        /* Final carry must be exactly 1. */
        if (rem - c == 0 && c != 1) continue;

        chain[pos] = c;
        dfs(c, rem - c, pos + 1, chain, accum, B, total_terms);
    }
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;          /* a(0) = 1 */

    idx_t B = n + 1;                      /* base (n+1) */
    seq_t total_terms = (seq_t)n * 7 + 1; /* L = 7n+1 */

    seq_t result = 0;
    idx_t chain[7];                       /* maximum 7 carries because all ≥1 */

    dfs(0, 7, 0, chain, &result, B, total_terms);
    return result;
}
