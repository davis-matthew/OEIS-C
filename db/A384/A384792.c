/*
 * A384792  Smallest number with a n-character traditional Japanese notation.
 * https://oeis.org/A384792
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, fini, full
 * Formula:   * a(8p+1) = '1' + '1221'^p * a(8p+2) = '11' + '1221'^p * a(8p+3)
 *            = '21' + '1221'^p * a(8p+4) = '121' + '1221'^p * a(8p+5) = '221'
 *            + '1221'^p * a(8p+6) = '1121' + '1221'^p * a(8p+7) = '1221' +
 *            '1221'^p * a(8p) = '11121' + '1221'^(p-1)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 19, u64 n <= 39, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */

seq_t a(idx_t n)
{
    static const char *prefix[8] = {
        "11121", /* r = 0   (n % 8 == 0) */
        "1",     /* r = 1   */
        "11",    /* r = 2   */
        "21",    /* r = 3   */
        "121",   /* r = 4   */
        "221",   /* r = 5   */
        "1121",  /* r = 6   */
        "1221"   /* r = 7   */
    };
    const char block[] = "1221";

    idx_t r = n % 8;
    idx_t p = n / 8;
    idx_t blockCnt = (r == 0) ? (p > 0 ? p - 1 : 0) : p;

    seq_t val = 0;
    size_t i;

    /* append prefix */
    for (i = 0; prefix[r][i] != '\0'; ++i)
        val = val * (seq_t)10 + (seq_t)(prefix[r][i] - '0');

    /* repeat the block "1221" */
    for (idx_t b = 0; b < blockCnt; ++b)
        for (i = 0; block[i] != '\0'; ++i)
            val = val * (seq_t)10 + (seq_t)(block[i] - '0');

    return val;
}
