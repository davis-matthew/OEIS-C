/*
 * A285400  Start with a single cell at coordinates (0, 0, 0), then
 *          iteratively subdivide the grid into 3 X 3 X 3 cells and remove the
 *          cells whose sum of modulo 2 coordinates is 0 or 3; a(n) is the
 *          number of cells after n iterations.
 * https://oeis.org/A285400
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(1) = 18, a(n) = 21 * a(n-1) for n ≥ 2
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *pow21 = NULL;
static idx_t pow21_max = 0;

static void ensure_pow21(idx_t need)
{
    if (pow21_max >= need)
        return;

    idx_t new_max = need;
    seq_t *tmp = (seq_t *)realloc(pow21, new_max * sizeof(seq_t));
    if (!tmp)
        abort();

    if (pow21_max == 0) {
        tmp[0] = (seq_t)1;
        pow21_max = 1;
    }

    for (idx_t i = pow21_max; i < new_max; ++i)
        tmp[i] = tmp[i - 1] * (seq_t)21;

    pow21 = tmp;
    pow21_max = new_max;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)18;

    ensure_pow21(n);           /* need powers up to 21^(n-1) */
    seq_t res = (seq_t)18 * pow21[n - 1];
    return res;
}
