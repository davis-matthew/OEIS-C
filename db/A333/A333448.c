/*
 * A333448  Smallest positive divisibility coefficient of A045572(n).
 * https://oeis.org/A333448
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-4) - a(n-8) (for n > 8) Initial values: a(1)=1,
 *            a(2)=1, a(3)=5, a(4)=1, a(5)=10, a(6)=4, a(7)=12, a(8)=2
 * Range:     u8 n <= 116, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The first eight terms are needed to start the recurrence. */
    const seq_t init[8] = {1, 1, 5, 1, 10, 4, 12, 2};

    if (n <= 8)
        return init[n - 1];

    /* The recurrence uses the last eight values:
       At step i the array holds a[i-8] … a[i-1].
       a[i-4] is always at index 4 and a[i-8] at index 0. */
    seq_t last8[8] = {1, 1, 5, 1, 10, 4, 12, 2};

    for (idx_t i = 9; i <= n; ++i) {
        seq_t new_val = 2 * last8[4] - last8[0];   /* a[i] = 2*a[i-4] - a[i-8] */

        /* Shift the array left by one and store the new value at the end. */
        for (int j = 0; j < 7; ++j)
            last8[j] = last8[j + 1];
        last8[7] = new_val;
    }

    return last8[7];
}
