/*
 * A371268  If the ternary expansion of n starts with a 1, replace 1's by 2's,
 *          2's by 0's, 0's by 1's, if it starts with a 2, replace 2's by 1's,
 *          1's by 0's, 0's by 2's.
 * https://oeis.org/A371268
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 242, u16 n <= 6560, u32 n <= 6560, u64 n <= 6560,
 *            u128 n <= 6560, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Transformation of ternary digits based on leading digit:
   If the leading ternary digit is 1, map 1→2, 2→0, 0→1.
   If the leading ternary digit is 2, map 2→1, 1→0, 0→2. */

seq_t a(idx_t n)
{
    /* Special case n == 0 */
    if (n == 0) return 0;

    /* Determine the leading ternary digit of n. */
    idx_t tmp = n;
    while (tmp >= 3)
        tmp /= 3;            /* tmp now holds the most significant digit (1 or 2) */

    /* Mapping tables */
    static const uint8_t map1[3] = {1, 2, 0}; /* for leading digit 1 */
    static const uint8_t map2[3] = {2, 0, 1}; /* for leading digit 2 */

    const uint8_t (*map)[3] = (tmp == 1) ? &map1 : &map2;

    /* Extract digits of n in base 3 (least significant first). */
    uint8_t digits[64];          /* 3^40 > 2^64, so 64 is safe */
    int len = 0;
    idx_t temp = n;
    while (temp > 0) {
        digits[len++] = (uint8_t)(temp % 3);
        temp /= 3;
    }

    /* Convert using the chosen mapping, processing digits from most significant to least. */
    seq_t result = 0;
    for (int i = len - 1; i >= 0; --i) {
        uint8_t d = digits[i];
        result = result * 3 + (*map)[d];
    }

    return result;
}
