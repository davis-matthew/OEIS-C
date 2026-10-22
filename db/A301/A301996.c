/*
 * A301996  Number of nX5 0..1 arrays with every element equal to 1, 2 or 4
 *          horizontally or antidiagonally adjacent elements, with upper left
 *          element zero.
 * https://oeis.org/A301996
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is defined by the 11‑term linear recurrence a(n) =
 *            18*a(n−1) – 141*a(n−2) + 661*a(n−3) – 2058*a(n−4) + 4393*a(n−5)
 *            – 6412*a(n−6) + 6060*a(n−7) – 3280*a(n−8) + 640*a(n−9) +
 *            192*a(n−10) – 64*a(n−11) for n > 12, with initial values a(1)=3,
 *            a(2)=18, a(3)=84, a(4)=468, a(5)=2856, a(6)=17800, a(7)...
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Base case: index zero is not used. */
    if (n == 0) return (seq_t)0;

    /* For n ≤ 12, return the known initial value truncated to seq_t. */
    if (n <= 12) {
        switch (n) {
            case 1:  return (seq_t)3ULL;
            case 2:  return (seq_t)18ULL;
            case 3:  return (seq_t)84ULL;
            case 4:  return (seq_t)468ULL;
            case 5:  return (seq_t)2856ULL;
            case 6:  return (seq_t)17800ULL;
            case 7:  return (seq_t)110036ULL;
            case 8:  return (seq_t)674984ULL;
            case 9:  return (seq_t)4130664ULL;
            case 10: return (seq_t)25284068ULL;
            case 11: return (seq_t)154853080ULL;
            case 12: return (seq_t)948637896ULL;
            default: return (seq_t)0;  /* unreachable */
        }
    }

    /* Allocate a temporary array of size n+1.  The index 0 slot is unused. */
    size_t sz = (size_t)(n + 1);
    seq_t *buf = (seq_t *)malloc(sz * sizeof(seq_t));
    if (!buf) return (seq_t)0;  /* allocation failed – unlikely for n ≤ 10000 */

    /* Initialize the known first twelve terms. */
    buf[0] = (seq_t)0;
    buf[1] = (seq_t)3ULL;
    buf[2] = (seq_t)18ULL;
    buf[3] = (seq_t)84ULL;
    buf[4] = (seq_t)468ULL;
    buf[5] = (seq_t)2856ULL;
    buf[6] = (seq_t)17800ULL;
    buf[7] = (seq_t)110036ULL;
    buf[8] = (seq_t)674984ULL;
    buf[9] = (seq_t)4130664ULL;
    buf[10] = (seq_t)25284068ULL;
    buf[11] = (seq_t)154853080ULL;
    buf[12] = (seq_t)948637896ULL;

    /* Compute the remaining terms using the recurrence. */
    for (idx_t i = 13; i <= n; ++i) {
        seq_t pos = (seq_t)18 * buf[i - 1];
        pos += (seq_t)661 * buf[i - 3];
        pos += (seq_t)4393 * buf[i - 5];
        pos += (seq_t)6060 * buf[i - 7];
        pos += (seq_t)640 * buf[i - 9];
        pos += (seq_t)192 * buf[i - 10];

        seq_t neg = (seq_t)141 * buf[i - 2];
        neg += (seq_t)2058 * buf[i - 4];
        neg += (seq_t)6412 * buf[i - 6];
        neg += (seq_t)3280 * buf[i - 8];
        neg += (seq_t)64  * buf[i - 11];

        buf[i] = pos - neg;  /* unsigned subtraction wraps modulo 2^k */
    }

    seq_t result = buf[n];
    free(buf);
    return result;
}
