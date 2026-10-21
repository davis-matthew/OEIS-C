/*
 * A246839  Number of trailing zeros in A002109(n).
 * https://oeis.org/A246839
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, changed
 * Formula:   a(n) = sum_{i=1..n} i * v_5(i) * where v_5(i) is the exponent of
 *            the highest power of 5 dividing i. * This implementation uses an
 *            O(n/5) loop, suitable for n up to about 10 000 * within the time
 *            limit and works for any seq_t size
 * Range:     u8 n <= 49, u16 n <= 724, u32 n <= 999, u64 n <= 999,
 *            u128 n <= 999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t m = n / 5;          /* floor(n/5) */
    seq_t sum = 0;

    for (idx_t k = 1; k <= m; ++k) {
        idx_t t = k;
        idx_t v = 0;
        while ((t % 5) == 0) { /* compute v_5(k) */
            t /= 5;
            ++v;
        }
        /* k * (v+1) and accumulate */
        sum += (seq_t)k * ((seq_t)v + 1);
    }

    return sum * 5;
}
