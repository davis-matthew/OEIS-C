/*
 * A254126  The number of tilings of a 5 X n rectangle using integer length
 *          rectangles with at least one length of size 1, i.e., tiles are 1 X
 *          1, 1 X 2, ..., 1 X n, 2 X 1, 3 X 1, 4 X 1, 5 X 1.
 * https://oeis.org/A254126
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 74 a(n-1) - 1450 a(n-2) + 10672 a(n-3) - 34214 a(n-4) +
 *            50814 a(n-5) - 34671 a(n-6) + 9772 a(n-7) - 936 a(n-8) with
 *            initial values a(0)=1, a(1)=16, a(2)=533, a(3)=22873,
 *            a(4)=1064576, a(5)=50796983, a(6)=2441987149, a(7)=117656540512
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t capacity = 0;
    static idx_t last = 0;  /* largest n for which cache[n] is valid */

    /* Grow storage if necessary */
    if ((size_t)n >= capacity) {
        size_t new_cap = capacity ? capacity * 2 : 8;
        while (new_cap <= (size_t)n) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        capacity = new_cap;
    }

    /* Initialize base cases once */
    if (last == 0 && capacity >= 8) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)16;
        cache[2] = (seq_t)533;
        cache[3] = (seq_t)22873;
        cache[4] = (seq_t)1064576;
        cache[5] = (seq_t)50796983;
        cache[6] = (seq_t)2441987149;
        cache[7] = (seq_t)117656540512;
        last = 7;
    }

    /* Compute values up to n if not yet done */
    if (n > last) {
        for (idx_t i = last + 1; i <= n; ++i) {
            seq_t sum_pos = 0;
            seq_t sum_neg = 0;

            /* Positive‑signed terms */
            sum_pos += cache[i-1] * (seq_t)74;
            sum_pos += cache[i-3] * (seq_t)10672;
            sum_pos += cache[i-5] * (seq_t)50814;
            sum_pos += cache[i-7] * (seq_t)9772;

            /* Negative‑signed terms */
            sum_neg += cache[i-2] * (seq_t)1450;
            sum_neg += cache[i-4] * (seq_t)34214;
            sum_neg += cache[i-6] * (seq_t)34671;
            sum_neg += cache[i-8] * (seq_t)936;

            /* Result is sum_pos - sum_neg; it is guaranteed positive */
            cache[i] = sum_pos - sum_neg;
        }
        last = n;
    }

    return cache[n];
}
