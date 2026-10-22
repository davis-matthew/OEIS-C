/*
 * A340270  a(n) = (n')^A054054(n), where n' is the number resulting from
 *          removing the rightmost occurrence of the smallest digit of n,
 *          A054054(n).
 * https://oeis.org/A340270
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = A340184(n)^A054054(n)
 * Range:     u8 n <= 36, u16 n <= 66, u32 n <= 585, u64 n <= 7776,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A340184(n) is n with its rightmost smallest digit removed;   */
/* A054054(n) is the smallest decimal digit of n.              */

seq_t a(idx_t n)
{
    /* Handle the off‑by‑one offset: a(1) corresponds to n==1. */
    if (n == 0)
        return (seq_t)0;              /* safe guard, not used in tests */

    /* Store decimal digits – least significant first. */
    unsigned digits[20];
    int len = 0;
    idx_t tmp = n;
    if (tmp == 0) {                  /* special case for n == 0 */
        digits[0] = 0;
        len = 1;
    } else {
        while (tmp > 0) {
            digits[len++] = (unsigned)(tmp % 10);
            tmp /= 10;
        }
    }

    /* Find the smallest digit. */
    unsigned min_digit = 9;
    for (int i = 0; i < len; ++i)
        if (digits[i] < min_digit)
            min_digit = digits[i];

    /* Find the rightmost (least significant) occurrence. */
    int rem_index = -1;              /* index to delete */
    for (int i = 0; i < len; ++i) {
        if (digits[i] == min_digit) {
            rem_index = i;
            break;
        }
    }

    /* Build n' by concatenating the remaining digits. */
    seq_t np = (seq_t)0;
    for (int k = len - 1; k >= 0; --k) {
        if (k == rem_index)
            continue;
        np = np * 10 + (seq_t)digits[k];
    }

    /* Compute (n')^min_digit. */
    if (min_digit == 0)
        return (seq_t)1;             /* any base to the power 0 is 1 */

    seq_t result = (seq_t)1;
    for (uint64_t i = 0; i < (uint64_t)min_digit; ++i)
        result *= np;

    return result;
}
