/*
 * A378833  Number of tripling steps in the 3x-1 trajectory from n to the
 *          minimum of its cycle, or -1 if n never reaches a cycle.
 * https://oeis.org/A378833
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of tripling steps (3x-1) in the 3x-1 trajectory
 *            from n until the trajectory first reaches one of the known cycle
 *            minima 1, 5, or 17. Returns –1 if the sequence detours into a
 *            cycle that does not contain any of these minima (in practice a
 *            hard limit is used)
 * Range:     u8 n <= 32, u16 n <= 3328, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Known cycle minima */
    const seq_t MIN1 = 1;
    const seq_t MIN5 = 5;
    const seq_t MIN17 = 17;

    /* A conservative step limit to avoid infinite loops for unproven
       cases.  For n ≤ 10000 the actual number of steps is far smaller
       than this limit. */
    const idx_t MAX_STEPS = 20000;

    seq_t x = (seq_t)n;   /* current value */
    seq_t count = 0;      /* number of 3x-1 (tripling) steps */

    for (idx_t i = 0; i < MAX_STEPS; ++i) {
        /* Stop as soon as we hit a cycle minimum. */
        if (x == MIN1 || x == MIN5 || x == MIN17) {
            return count;
        }

        /* Apply the 3x-1 rule if x is odd. */
        if (x & 1) {
            x = 3 * x - 1;
            ++count;
        } else {
            x = x >> 1;   /* x = x / 2 for even x */
        }
    }

    /* Exceeded step limit – treat as a non‑cyclic case. */
    return (seq_t)-1;
}
