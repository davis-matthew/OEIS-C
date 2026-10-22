/*
 * A371263  The run lengths transform of the balanced ternary expansion of n
 *          corresponds to the run lengths transform of the binary expansion
 *          of a(n).
 * https://oeis.org/A371263
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) equals the integer whose binary runs of 0/1 have the same
 *            lengths as the runs of digits in the balanced‑ternary expansion
 *            of n
 * Range:     u8 n <= 3280, u16 n <= 9841, u32 n <= 9841, u64 n <= 9841,
 *            u128 n <= 9841, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;              /* a(0) = 0 */

    /* Compute balanced ternary digits (least significant first).  Digits are -1, 0, or 1. */
    int digits[64];                    /* uint64_t has at most log3(2^64) ≈ 41 digits */
    int len = 0;
    uint64_t tmp = (uint64_t)n;

    while (tmp != 0) {
        int r = tmp % 3;
        if (r == 0) {
            digits[len++] = 0;
            tmp /= 3;
        } else if (r == 1) {
            digits[len++] = 1;
            tmp = (tmp - 1) / 3;
        } else {                      /* r == 2 */
            digits[len++] = -1;
            tmp = (tmp + 1) / 3;
        }
    }

    /* Reverse to most significant first and compute run lengths. */
    int run[64];
    int rlen = 0;
    int cur = digits[len - 1];
    int count = 1;
    for (int i = len - 2; i >= 0; --i) {
        if (digits[i] == cur) {
            ++count;
        } else {
            run[rlen++] = count;
            cur = digits[i];
            count = 1;
        }
    }
    run[rlen++] = count;

    /* Build the binary integer from these run lengths.  The first run starts with bit 1
       (since binary representations cannot start with 0).  Alternate bits after each run. */
    seq_t result = 0;
    int bit = 1;
    for (int i = 0; i < rlen; ++i) {
        int L = run[i];
        result <<= L;                         /* shift left to make room for the run */
        if (bit) {
            result |= ((seq_t)1 << L) - 1;     /* append L ones if current bit is 1 */
        }
        bit ^= 1;                             /* toggle between 0 and 1 */
    }

    return result;
}
