/*
 * A319806  a(n) = A319723(n) + A319654(n).
 * https://oeis.org/A319806
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = A319723(n) + A319654(n), i.e. (base-6 digits of n sorted
 *            descending, read in base 6) + (base-6 digits sorted ascending,
 *            read in base 6)
 * Range:     u8 n <= 58, u16 n <= 7775, u32 n <= 7775, u64 n <= 7775,
 *            u128 n <= 7775, big any n
 * Validated: standard, pass, 81 terms through n = 7519 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t A319806_desc(idx_t n)
{
    unsigned int counts[6] = {0, 0, 0, 0, 0, 0};
    idx_t temp = n;
    while (temp != 0) {
        ++counts[(unsigned int)(temp % 6)];
        temp /= 6;
    }
    seq_t result = 0;
    for (int d = 5; d >= 0; --d) {
        for (unsigned int i = 0; i < counts[d]; ++i) {
            result = result * 6 + (seq_t)d;
        }
    }
    return result;
}

static seq_t A319806_asc(idx_t n)
{
    unsigned int counts[6] = {0, 0, 0, 0, 0, 0};
    idx_t temp = n;
    while (temp != 0) {
        ++counts[(unsigned int)(temp % 6)];
        temp /= 6;
    }
    if (n == 0) counts[0] = 1;
    seq_t result = 0;
    for (unsigned int d = 0; d < 6; d++) {
        for (unsigned int i = 0; i < counts[d]; ++i) {
            result = result * 6 + (seq_t)d;
        }
    }
    return result;
}

seq_t a(idx_t n)
{
    return A319806_desc(n) + A319806_asc(n);
}
