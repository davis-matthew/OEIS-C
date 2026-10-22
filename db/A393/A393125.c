/*
 * A393125  Maximum of the orbit of n under the map x -> 5x+1 if odd else x/2;
 *          -1 if the orbit is unbounded.
 * https://oeis.org/A393125
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more, hard
 * Formula:   a(n) = maximum of the orbit of n under the map x -> 5x + 1 if x
 *            is odd, else x / 2; return -1 if the orbit is unbounded
 *            (detected by exceeding a step limit)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline seq_t step(seq_t x)
{
    return (x & 1) ? (5 * x + 1) : (x >> 1);
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* Maximum number of steps allowed before guessing the orbit is
       unbounded.  This limit is safe for n <= 10000 and keeps the
       routine fast. */
    const idx_t MAX_STEPS = 1000000;

    seq_t maxv  = (seq_t)n;
    seq_t tortoise = step((seq_t)n);
    if (tortoise > maxv) maxv = tortoise;
    seq_t hare = step(tortoise);
    if (hare > maxv) maxv = hare;

    idx_t steps = 0;
    /* Floyd’s cycle‑finding: move tortoise one step, hare two steps. */
    while (tortoise != hare)
    {
        if (++steps > MAX_STEPS)               /* orbit divergent */
            return (seq_t)-1;

        tortoise = step(tortoise);
        if (tortoise > maxv) maxv = tortoise;

        hare = step(step(hare));
        if (hare > maxv) maxv = hare;
    }

    /* Find the entry point of the cycle. */
    seq_t start = (seq_t)n;
    seq_t cur   = tortoise;
    while (start != cur)
    {
        start = step(start);
        if (start > maxv) maxv = start;
        cur   = step(cur);
        if (cur   > maxv) maxv = cur;
    }

    /* Maximum over one full cycle. */
    seq_t v = step(start);
    while (v != start)
    {
        if (v > maxv) maxv = v;
        v = step(v);
    }

    return maxv;
}
