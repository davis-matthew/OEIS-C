/*
 * A295869  Numbers not divisible by 2, 3 or 5 (A007775) with digital root 8.
 * https://oeis.org/A295869
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = a(n-1) + a(n-4) - a(n-5) for n>5 Initial values: a(1)=17,
 *            a(2)=53, a(3)=71, a(4)=89, a(5)=107
 * Range:     u8 n <= 11, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* base terms for n = 1..5 */
    static const seq_t base[5] = {17, 53, 71, 89, 107};

    /* Return directly if within the initial block */
    if (n <= 5)
        return base[n - 1];

    /* Temporary storage for the five most recent values:
       arr[0] = a(i-5), arr[1] = a(i-4), arr[2] = a(i-3),
       arr[3] = a(i-2), arr[4] = a(i-1) */
    seq_t arr[5];
    for (int j = 0; j < 5; ++j)
        arr[j] = base[j];

    /* Iterate from i = 6 up to the requested index n */
    for (idx_t i = 6; i <= n; ++i) {
        /* Apply the recurrence: a(i) = a(i-1) + a(i-4) - a(i-5) */
        seq_t next = arr[1] + arr[4] - arr[0];

        /* Shift the window forward one step */
        for (int j = 0; j < 4; ++j)
            arr[j] = arr[j + 1];
        arr[4] = next;
    }

    /* arr[4] now holds a(n) */
    return arr[4];
}
