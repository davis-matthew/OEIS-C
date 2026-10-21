/*
 * A272764  Number of positive roots in reflection group E_n.
 * https://oeis.org/A272764
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) equals the number of positive roots of the reflection group
 *            E_n. It is the sum of the exponents of the corresponding root
 *            system. For n = 3 we return the known value 3. For n = 4 (type
 *            A₄) the exponents are 1,2,3,4 (sum = n · (n + 1)/2). For n = 5
 *            (type D₅) the exponents are 1,3,4,5,7 (sum =...
 * Range:     u8 n <= 8, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result;
    switch (n) {
        case 3:
            return (seq_t)3;
        case 4:
            /* A₄ exponents: 1,2,3,4 */
            result = ((seq_t)n) * ((seq_t)n + 1) / 2;  /* 10 */
            return result;
        case 5:
            /* D₅ exponents: 1,3,4,5,7 → sum = n · (n − 1) */
            result = (seq_t)(n * (n - 1));  /* 20 */
            return result;
        case 6: {
            /* E₆ exponents: 1,4,5,7,8,11 */
            const seq_t exp[] = {1, 4, 5, 7, 8, 11};
            seq_t sum = 0;
            for (int i = 0; i < 6; ++i) sum += exp[i];
            return sum;             /* 36 */
        }
        case 7: {
            /* E₇ exponents: 1,5,7,9,11,13,17 */
            const seq_t exp[] = {1, 5, 7, 9, 11, 13, 17};
            seq_t sum = 0;
            for (int i = 0; i < 7; ++i) sum += exp[i];
            return sum;             /* 63 */
        }
        case 8: {
            /* E₈ exponents: 1,7,11,13,17,19,23,29 */
            const seq_t exp[] = {1, 7, 11, 13, 17, 19, 23, 29};
            seq_t sum = 0;
            for (int i = 0; i < 8; ++i) sum += exp[i];
            return sum;             /* 120 */
        }
        default:
            /* The reflection group E_n is not defined for n > 8, so we return 0. */
            return 0;
    }
}
