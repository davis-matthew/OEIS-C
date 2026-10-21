/*
 * A289411  a(n) = Sum_{k=0..n} sign(A007953(5*k) - A007953(k)).
 * https://oeis.org/A289411
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = Sum_{k=0..n} sign( digit_sum(5*k) - digit_sum(k) )
 * Range:     u8 n <= 51, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t digit_sum(seq_t x)
{
    seq_t sum = 0;
    while (x != 0) {
        sum += x % 10;
        x   /= 10;
    }
    return sum;
}

seq_t a(idx_t n)
{
    seq_t total = 0;
    for (idx_t k = 0; k <= n; ++k) {
        seq_t fivek = (seq_t)k * (seq_t)5;
        seq_t d5    = digit_sum(fivek);
        seq_t d     = digit_sum((seq_t)k);
        if (d5 > d) {
            total += 1;
        } else if (d5 < d) {
            total -= 1;   // assumes the result never becomes negative
        }
    }
    return total;
}
