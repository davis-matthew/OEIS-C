/*
 * A323999  Start with 348, then successively apply /12, -11, *10, /9, -8, *7,
 *          /6, -5, *4, /3, -2, *1.
 * https://oeis.org/A323999
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 13, u32 n <= 13, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A323999: start with 348 and repeatedly apply the
   operations /12, -11, *10, /9, -8, *7, /6, -5, *4, /3, -2, *1
   to the previous term. */
seq_t a(idx_t n) {
    /* a(1) = 348, for n > 1 apply the 12‑step cycle n‑1 times. */
    if (n == 0) return 0;          /* offset is 1, so this case is unused */
    seq_t val = 348;
    for (idx_t i = 1; i < n; ++i) {
        idx_t j = (i - 1) % 12;    /* choose operation for this step */
        switch (j) {
            case 0: val = val / 12; break;
            case 1: val = val - 11; break;
            case 2: val = val * 10; break;
            case 3: val = val / 9; break;
            case 4: val = val - 8; break;
            case 5: val = val * 7; break;
            case 6: val = val / 6; break;
            case 7: val = val - 5; break;
            case 8: val = val * 4; break;
            case 9: val = val / 3; break;
            case 10: val = val - 2; break;
            case 11: val = val * 1; break;
        }
    }
    return val;
}
