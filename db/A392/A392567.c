/*
 * A392567  Sum of the smallest parts in the partitions of n into 3 parts
 *          whose middle part is even.
 * https://oeis.org/A392567
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{j=1..⌊n/3⌋} Sum_{k=j..⌊(n-j)/2⌋} j * ((k+1) mod 2)
 * Range:     u8 n <= 36, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This is equivalent to a(n) = Σ_{j=1}^{⌊n/3⌋} j * (# even k in [j, ⌊(n−j)/2⌋]) */

seq_t a(idx_t n) {
    seq_t sum = 0;
    idx_t max_j = n / 3;                /* floor(n/3) */
    for (idx_t j = 1; j <= max_j; ++j) {
        idx_t hi = (n - j) / 2;         /* floor((n−j)/2) */
        if (hi < j) continue;           /* empty range, skip */
        idx_t lo = j;
        /* first even k in the interval */
        idx_t lo_even = (lo % 2 == 0) ? lo : lo + 1;
        if (lo_even > hi) continue;     /* no even k */
        idx_t count = ((hi - lo_even) / 2) + 1;  /* number of evens */
        sum += (seq_t)j * (seq_t)count;
    }
    return sum;
}
