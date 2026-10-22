/*
 * A385334  Cat number of complete graphs on n vertices.
 * https://oeis.org/A385334
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(1)=0, a(2)=1, a(3)=2 a(n)= ⌊n/2⌋·⌈n/2⌉ + a(⌈n/2⌉), for n≥4
 * Range:     u8 n <= 27, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return 0;
    if (n == 2) return 1;
    if (n == 3) return 2;

    seq_t sum = 0;
    idx_t cur = n;

    while (cur > 3) {
        idx_t hf = cur / 2;          /* ⌊cur/2⌋ */
        idx_t hc = cur - hf;         /* ⌈cur/2⌉ */
        sum += (seq_t)hf * (seq_t)hc;
        cur = hc;
    }

    /* add the remaining base value */
    if (cur == 1)
        sum += 0;
    else if (cur == 2)
        sum += 1;
    else /* cur == 3 */
        sum += 2;

    return sum;
}
