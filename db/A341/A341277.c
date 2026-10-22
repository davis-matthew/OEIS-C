/*
 * A341277  Number of length-n binary mesosome-avoiding strings.
 * https://oeis.org/A341277
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(4k) = (4k³ + 15k² + 41k - 12) / 3 a(4k+1) = (4k³ + 18k² + 50k)
 *            / 3 a(4k+2) = (4k³ + 21k² + 59k + 12) / 3 a(4k+3) = (4k³ + 24k²
 *            + 68k + 30) / 3 (valid for n ≥ 5; for n < 5 the values are
 *            1,2,4,8,14)
 * Range:     u8 n <= 17, u16 n <= 141, u32 n <= 4152, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n < 5) {
        /* small indices – return the known values */
        switch ((unsigned int)n) {
            case 0: return (seq_t)1;
            case 1: return (seq_t)2;
            case 2: return (seq_t)4;
            case 3: return (seq_t)8;
            case 4: return (seq_t)14;
            default: return (seq_t)0; /* never reached */
        }
    }
    /* write n as 4k + i with 0 ≤ i < 4 */
    seq_t k = (seq_t)(n / 4);
    unsigned int i = (unsigned int)(n % 4);   /* use a built‑in type for the switch */

    seq_t res;
    switch (i) {
        case 0:
            res = ( (seq_t)4 * k * k * k
                  + (seq_t)15 * k * k
                  + (seq_t)41 * k
                  - (seq_t)12 ) / (seq_t)3;
            break;
        case 1:
            res = ( (seq_t)4 * k * k * k
                  + (seq_t)18 * k * k
                  + (seq_t)50 * k ) / (seq_t)3;
            break;
        case 2:
            res = ( (seq_t)4 * k * k * k
                  + (seq_t)21 * k * k
                  + (seq_t)59 * k
                  + (seq_t)12 ) / (seq_t)3;
            break;
        case 3:
            res = ( (seq_t)4 * k * k * k
                  + (seq_t)24 * k * k
                  + (seq_t)68 * k
                  + (seq_t)30 ) / (seq_t)3;
            break;
        default:
            res = (seq_t)0; /* never reached */
    }
    return res;
}
