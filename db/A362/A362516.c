/*
 * A362516  Number of vertex cuts in the n-gear graph.
 * https://oeis.org/A362516
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) =
 *            10*a(n-1)-34*a(n-2)+44*a(n-3)-13*a(n-4)-14*a(n-5)+8*a(n-6);
 *            Initial values: a(1)=1, a(2)=5, a(3)=51, a(4)=293, a(5)=1383,
 *            a(6)=6017
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle indices below the offset – behaviour undefined. */
    if (n < 1) return (seq_t)0;

    /* Base values for n = 1 … 6.  These are stored in a static
       array so that they can be used directly without any
       dynamic allocation. */
    static const seq_t base_vals[6] = {
        1,   /* a(1) */
        5,   /* a(2) */
        51,  /* a(3) */
        293, /* a(4) */
        1383,/* a(5) */
        6017 /* a(6) */
    };

    if (n <= 6)
        return base_vals[n - 1];

    /* Use a ring buffer of length 6 to hold the last six terms of
       the recurrence.  The indices are arranged so that
       ring[0] = a(n-6), ring[1] = a(n-5), …, ring[5] = a(n-1). */
    seq_t ring[6] = { base_vals[0], base_vals[1], base_vals[2],
                      base_vals[3], base_vals[4], base_vals[5] };

    /* Compute terms a(7) … a(n) iteratively. */
    for (idx_t i = 7; i <= n; ++i) {
        /* recurrence: 10*a(n-1)-34*a(n-2)+44*a(n-3)-13*a(n-4)-14*a(n-5)+8*a(n-6) */
        seq_t val = 10 * ring[5];
        val = val - 34 * ring[4];
        val = val + 44 * ring[3];
        val = val - 13 * ring[2];
        val = val - 14 * ring[1];
        val = val + 8 * ring[0];

        /* shift ring: drop a(n-6), shift others left, insert new value */
        seq_t tmp0 = ring[0];
        ring[0] = ring[1];
        ring[1] = ring[2];
        ring[2] = ring[3];
        ring[3] = ring[4];
        ring[4] = ring[5];
        ring[5] = val;
    }

    /* After the loop, ring[5] holds a(n). */
    return ring[5];
}
