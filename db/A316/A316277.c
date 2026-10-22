/*
 * A316277  Number of nX3 0..1 arrays with every element unequal to 1, 2, 3,
 *          4, 5 or 8 king-move adjacent elements, with upper left element
 *          zero.
 * https://oeis.org/A316277
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) + 16*a(n-2) + 5*a(n-3) - 50*a(n-4) - 91*a(n-5) -
 *            64*a(n-6) - 37*a(n-7) - 23*a(n-8) - 5*a(n-9) (n>10) initial
 *            values: a(1)=1, a(2)=25, a(3)=125, a(4)=846, a(5)=5321,
 *            a(6)=34017, a(7)=216805, a(8)=1382759, a(9)=8818011,
 *            a(10)=56235794
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* static cache up to n = 10000 (enough for all test cases) */
    static seq_t cache[10001] = {0};
    static idx_t computed_max = 0;

    /* initialise first 10 values on first call */
    if (!computed_max) {
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)25;
        cache[3] = (seq_t)125;
        cache[4] = (seq_t)846;
        cache[5] = (seq_t)5321;
        cache[6] = (seq_t)34017;
        cache[7] = (seq_t)216805;
        cache[8] = (seq_t)1382759;
        cache[9] = (seq_t)8818011;
        cache[10] = (seq_t)56235794;
        computed_max = 10;
    }

    /* If requested index already computed, return it */
    if (n <= computed_max) {
        return cache[n];
    }

    /* compute missing values up to index n */
    for (idx_t i = computed_max + 1; i <= n; ++i) {
        /* recurrence requires i >= 11; for i <= 10 we already have values */
        seq_t v = 0;
        v += (seq_t)4   * cache[i - 1];
        v += (seq_t)16  * cache[i - 2];
        v += (seq_t)5   * cache[i - 3];
        v -= (seq_t)50  * cache[i - 4];
        v -= (seq_t)91  * cache[i - 5];
        v -= (seq_t)64  * cache[i - 6];
        v -= (seq_t)37  * cache[i - 7];
        v -= (seq_t)23  * cache[i - 8];
        v -= (seq_t)5   * cache[i - 9];
        cache[i] = v;
    }
    computed_max = n;
    return cache[n];
}
