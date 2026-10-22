/*
 * A305214  Numbers k such that A305212(k) is not zero.
 * https://oeis.org/A305214
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 57, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: nth number divisible by 7 or 9 (sorted, no duplicates) */
seq_t a(idx_t n)
{
    /* For the defined offset (a(1)=7) we assume n>=1; return 0 for n==0. */
    if (n == 0) return 0;

    idx_t i = 1, j = 1;              /* counters for multiples of 7 and 9 */
    seq_t next7 = (seq_t)7 * i;
    seq_t next9 = (seq_t)9 * j;
    seq_t last = 0;

    for (idx_t k = 1; k <= n; ++k) {
        if (next7 < next9) {
            last = next7;
            ++i;
            next7 = (seq_t)7 * i;
        } else if (next9 < next7) {
            last = next9;
            ++j;
            next9 = (seq_t)9 * j;
        } else {                     /* duplicate: next7 == next9 */
            last = next7;
            ++i;
            next7 = (seq_t)7 * i;
            ++j;
            next9 = (seq_t)9 * j;
        }
    }
    return last;
}
