/*
 * A343644  a(n) is the sum of the number of occurrences of each digit of n up
 *          to and including that digit.
 * https://oeis.org/A343644
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base, look
 * Formula:   a(n) is the sum, over the decimal digits of n, of the (1‑based)
 *            index of each digit in the sequence formed by writing 1,2,3,…,n
 *            in decimal
 * Range:     u8 n <= 180, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) as defined in OEIS A343644.  The implementation
   counts the occurrences of each digit in 1..n‑1 and then
   traverses the digits of n from most to least significant,
   adding the appropriate cumulative index for each digit.  */
seq_t a(idx_t n)
{
    /* Count digit occurrences in numbers 1 .. n-1. */
    idx_t before[10];
    for (int i = 0; i < 10; ++i) before[i] = 0;

    for (idx_t num = 1; num < n; ++num) {
        idx_t x = num;
        if (x == 0) {
            before[0]++;          /* never happens for num≥1 */
            continue;
        }
        while (x > 0) {
            before[x % 10]++;     /* count digit */
            x /= 10;
        }
    }

    /* Extract digits of n into an array (least significant first). */
    uint8_t ndigits[32];           /* enough for 64‑bit numbers */
    int len = 0;
    idx_t t = n;
    do {
        ndigits[len++] = (uint8_t)(t % 10);
        t /= 10;
    } while (t > 0);

    /* Sum the indices of each digit as it appears in n. */
    idx_t in_n[10] = {0};
    seq_t result = (seq_t)0;

    for (int i = len - 1; i >= 0; --i) {
        int d = ndigits[i];
        seq_t idx = (seq_t)(before[d] + in_n[d] + 1);
        result += idx;
        in_n[d]++;
    }

    return result;
}
