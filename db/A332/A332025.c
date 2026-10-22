/*
 * A332025  Sum of the lengths of the longest runs of 0, 1, and 2 in the
 *          ternary expression of n.
 * https://oeis.org/A332025
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = sum of the lengths of the longest runs of 0, 1, and 2 in
 *            the ternary expression of n
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;               /* ternary "0" → longest run of 0 is 1, others 0 */

    idx_t temp = n;
    int prev = -1;                    /* previous digit (0,1,2) or -1 for none */
    idx_t curLen = 0;
    idx_t maxLen[3] = {0, 0, 0};       /* longest run for each digit */

    while (temp) {
        int digit = temp % 3;          /* next ternary digit (least significant) */
        if (digit == prev) {
            curLen++;                  /* continuing current run */
        } else {
            if (prev != -1 && curLen > maxLen[prev])
                maxLen[prev] = curLen; /* update longest run for previous digit */
            curLen = 1;                /* start new run */
            prev = digit;
        }
        temp /= 3;
    }

    if (prev != -1 && curLen > maxLen[prev])        /* final run */
        maxLen[prev] = curLen;

    seq_t sum = (seq_t)maxLen[0] + (seq_t)maxLen[1] + (seq_t)maxLen[2];
    return sum;
}
