/*
 * A352328  Nonnegative numbers that are the sum of distinct Pell numbers
 *          (A000129).
 * https://oeis.org/A352328
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = ∑ b_k · A000129(k+1) where (b_k) is the binary expansion
 *            of n
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *pell = NULL;   /* P_0, P_1, P_2, … */
static size_t pell_cap = 0;  /* allocated length */

/* ensure pell array has at least 'need' elements (0 … need-1) */
static void
ensure_pell_sz(size_t need)
{
    if (need <= pell_cap)
        return;
    seq_t *ptr = (seq_t *)realloc(pell, need * sizeof(seq_t));
    if (!ptr)
        abort();                 /* memory exhausted – abort is safe */
    pell = ptr;
    /* initialize new elements */
    for (size_t i = pell_cap; i < need; ++i) {
        if (i == 0)
            pell[i] = (seq_t)0;
        else if (i == 1)
            pell[i] = (seq_t)1;
        else
            pell[i] = pell[i - 1] * (seq_t)2 + pell[i - 2];
    }
    pell_cap = need;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* determine number of bits needed */
    size_t bit = 0;
    idx_t tmp = n;
    while (tmp) {
        tmp >>= 1;
        ++bit;
    }
    /* we need pell[0 … bit] (indices up to bit) */
    ensure_pell_sz(bit + 1);

    seq_t result = (seq_t)0;
    for (size_t i = 0; i < bit; ++i) {
        if (n & ((idx_t)1 << i))
            result += pell[i + 1];   /* Pell_(i+1) */
    }
    return result;
}
