/*
 * A278038  Binary vectors not containing three consecutive 1's; or,
 *          representation of n in the tribonacci base.
 * https://oeis.org/A278038
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the tribonacci representation of n * written as a
 *            decimal number whose digits are the binary * tribonacci digits
 *            (0 or 1). The representation is unique * and contains no three
 *            consecutive 1's
 * Range:     u8 n <= 6, u16 n <= 23, u32 n <= 503, u64 n <= 9823,
 *            u128 n <= 9823, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static idx_t *tribs = NULL;    /* tribonacci numbers <= current max n   */
static size_t trib_len = 0;    /* number of stored tribonacci numbers    */
static size_t trib_cap = 0;    /* current allocation capacity           */

/* Ensure tribs[] contains all tribonacci numbers <= max_n.
 * tri[0] = 1, tri[1] = 2, tri[2] = 4, tri[i] = tri[i-1]+tri[i-2]+tri[i-3]   */
static void ensure_tribs(idx_t max_n)
{
    /* Initialise the first three values once */
    if (trib_len == 0) {
        trib_cap = 3;
        tribs = (idx_t *)malloc(trib_cap * sizeof(idx_t));
        tribs[0] = 1;
        tribs[1] = 2;
        tribs[2] = 4;
        trib_len = 3;
    }

    while (1) {
        if (trib_len < 3) break;          /* safety, should never happen */
        idx_t a = tribs[trib_len - 3];
        idx_t b = tribs[trib_len - 2];
        idx_t c = tribs[trib_len - 1];

        /* guard against overflow */
        if (UINT64_MAX - a - b < c) break;

        idx_t next = a + b + c;
        if (next > max_n) break;

        /* grow the array if necessary */
        if (trib_len >= trib_cap) {
            trib_cap = trib_cap * 2;
            tribs = (idx_t *)realloc(tribs, trib_cap * sizeof(idx_t));
        }
        tribs[trib_len++] = next;
    }
}

/* Convert n to its tribonacci representation and output that
 * representation as a decimal number (with digits 0/1).                 */
seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;
    }

    idx_t m = n;                       /* working copy of n */
    ensure_tribs(m);                   /* make sure tribs[] covers m */

    seq_t res = 0;
    /* Iterate from highest tribonacci index down to 0 */
    for (int i = (int)trib_len - 1; i >= 0; --i) {
        res = res * 10;                /* shift left in base 10 */

        if (m >= tribs[i]) {
            m -= tribs[i];
            res += 1;                  /* place a '1' digit */
        }
    }

    return res;
}
