/*
 * A304094  Number of Lucas numbers (A000204: 1, 3, 4, 7, 11, ... excluding 2)
 *          that divide n.
 * https://oeis.org/A304094
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of Lucas numbers (excluding the initial 2) that
 *            divide n
 * Range:     u8 n <= 1418, u16 n <= 8822, u32 n <= 8822, u64 n <= 8822,
 *            u128 n <= 8822, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* below the offset, value is irrelevant */
    seq_t count = 0;
    seq_t m = (seq_t)n;

    /* Lucas sequence: L0 = 2, L1 = 1, Lk = Lk-1 + Lk-2 */
    seq_t prev = 2;   /* L0 */
    seq_t curr = 1;   /* L1 */

    while (curr <= m) {
        if (m % curr == 0) ++count;

        /* avoid overflow on unsigned builds */
        if (sizeof(seq_t) <= 8) {
            seq_t max_val = (seq_t)(~(seq_t)0);
            if (prev > max_val - curr) break;   /* next Lucas would overflow */
        }

        seq_t next = prev + curr;
        prev = curr;
        curr = next;
    }
    return count;
}
