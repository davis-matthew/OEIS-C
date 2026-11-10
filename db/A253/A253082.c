/*
 * A253082  Partial sums of A246595.
 * https://oeis.org/A253082
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} A246595(k), where A246595(k) = (product of
 *            lengths of runs of 1's in binary expansion of k)^2, with
 *            A246595(0) = 1
 * Range:     u8 n <= 50, u16 n <= 1912, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Validated: standard, pass, 89 terms through n = 7333 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t A246595_a(idx_t n)
{
    seq_t prod = 1;
    idx_t i = n;
    idx_t run = 0;

    while (i > 0) {
        if (i & 1U) {
            run++;
        } else {
            if (run != 0) {
                prod *= (seq_t)run;
                run = 0;
            }
        }
        i >>= 1;
    }
    if (run != 0) {
        prod *= (seq_t)run;
    }

    return prod * prod;
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t k;
    for (k = 0; k <= n; k++) {
        sum += A246595_a(k);
        if (k == n) break;
    }
    return sum;
}
