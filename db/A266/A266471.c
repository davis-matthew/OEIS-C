/*
 * A266471  Number of 4 X n binary arrays with rows and columns
 *          lexicographically nondecreasing and column sums nonincreasing.
 * https://oeis.org/A266471
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) - 15*a(n-2) + 20*a(n-3) - 15*a(n-4) + 6*a(n-5) -
 *            a(n-6) for n > 6 with initial values: a(1)=5, a(2)=12, a(3)=29,
 *            a(4)=66, a(5)=137, a(6)=261 This implementation evaluates the
 *            recurrence iteratively and works for any n >= 1 in O(n) time,
 *            independent of compilation target
 * Range:     u8 n <= 5, u16 n <= 22, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    switch (n) {
    case 1: return (seq_t)5;
    case 2: return (seq_t)12;
    case 3: return (seq_t)29;
    case 4: return (seq_t)66;
    case 5: return (seq_t)137;
    case 6: return (seq_t)261;
    }

    /* Sliding window of the last six terms, indexed as
       v[0] = a(i-6), v[1] = a(i-5), ..., v[5] = a(i-1)   */
    seq_t v[6] = { (seq_t)5, (seq_t)12, (seq_t)29,
                   (seq_t)66, (seq_t)137, (seq_t)261 };

    for (idx_t i = 7; i <= n; ++i) {
        seq_t next = (seq_t)6 * v[5]
                   - (seq_t)15 * v[4]
                   + (seq_t)20 * v[3]
                   - (seq_t)15 * v[2]
                   + (seq_t)6  * v[1]
                   - v[0];
        /* Shift the window one position forward */
        v[0] = v[1];
        v[1] = v[2];
        v[2] = v[3];
        v[3] = v[4];
        v[4] = v[5];
        v[5] = next;
    }
    return v[5];
}
