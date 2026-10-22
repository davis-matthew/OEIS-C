/*
 * A316843  Column 1 of table A316841.
 * https://oeis.org/A316843
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest k such that Σ_{i=1}^{k} ⌊(i+1)²/4⌋ ≥ n
 * Range:     u8 n <= 497, u16 n <= 3000, u32 n <= 3000, u64 n <= 3000,
 *            u128 n <= 3000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (This uses A002620(k+1) = ⌊(k+1)²/4⌋). */
seq_t a(idx_t n)
{
    idx_t k = 1;          /* current block index (k starts at 1) */
    seq_t cumulative = 0; /* running total of terms up to current block */

    /* Continue adding blocks until cumulative count covers n. */
    while (cumulative < (seq_t)n)
    {
        seq_t kp1 = (seq_t)(k + 1);                 /* k+1 */
        seq_t block = (kp1 * kp1) / 4; /* ⌊(k+1)²/4⌋  = A002620(k+1) */
        cumulative += block;
        if (cumulative >= (seq_t)n)
            break;
        ++k; /* move to next k for next iteration */
    }

    return (seq_t)k;
}
