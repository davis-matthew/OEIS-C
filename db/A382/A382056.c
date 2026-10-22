/*
 * A382056  Remove every copy of the largest digit of n; if any digits remain,
 *          return the number formed by arranging the remaining digits in
 *          nondecreasing order. If no digits remain, return 0.
 * https://oeis.org/A382056
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look, base, easy
 * Formula:   a(n) = concatenate the digits of n in original order after *
 *            removing all occurrences of the largest digit; if no digits
 *            remain, * return 0
 * Range:     u8 n <= 2566, u16 n <= 9778, u32 n <= 9778, u64 n <= 9778,
 *            u128 n <= 9778, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle n == 0 explicitly.  The sequence starts at 1, but this
     * guard makes the function safe for any idx_t value. */
    if (n == 0)
        return 0;

    /* Extract the decimal digits of n into a fixed-size array.
     * digits[i] holds the i-th least significant digit. */
    int digits[20];
    int len = 0;
    idx_t tmp = n;
    while (tmp > 0) {
        digits[len++] = (int)(tmp % 10);
        tmp /= 10;
    }

    /* Find the largest digit in the number. */
    int maxdigit = 0;
    for (int i = 0; i < len; ++i)
        if (digits[i] > maxdigit)
            maxdigit = digits[i];

    /* Reconstruct the resulting number by scanning digits from most
     * significant to least significant, skipping any that equal the
     * largest digit. */
    seq_t res = 0;
    int any = 0;   /* flag: have we kept any digit? */
    for (int i = len - 1; i >= 0; --i) {
        if (digits[i] == maxdigit)
            continue;
        res = res * 10 + digits[i];
        any = 1;
    }

    /* If all digits were removed, the result should be 0. */
    if (!any)
        return 0;
    return res;
}
