/*
 * A322583  a(n) is the number of factorizations of n into factorial numbers
 *          larger than one; a(1) = 1.
 * https://oeis.org/A322583
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of unordered factorizations of n into factorials >
 *            1
 * Range:     u8 n <= 9833, u16 n <= 9833, u32 n <= 9833, u64 n <= 9833,
 *            u128 n <= 9833, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* static table of factorials > 1, up to the largest value that fits in idx_t   */
static idx_t  fac[64];
static size_t fac_cnt = 0;
static int    fac_ok   = 0;

/* initialise factorial table once.                                            */
static void
init_factorials(void)
{
    if (fac_ok) return;
    fac_ok = 1;

    /* compute factorials 2!, 3!, ... until overflow */
    idx_t prod = 1;
    for (idx_t i = 2; ; ++i)
    {
        /* protect against overflow: if new product < old product we exceeded capacity */
        idx_t next = prod * i;
        if (next < prod) break;
        fac[fac_cnt++] = next;
        prod = next;
        /* limit size of table so that array does not overflow */
        if (fac_cnt >= sizeof(fac) / sizeof(fac[0])) break;
    }
}

/* recursive helper: number of factorizations of cur using factorials
   with index >= start (non‑decreasing order).                           */
static seq_t
rec(idx_t cur, size_t start)
{
    if (cur == 1) return (seq_t)1;

    seq_t res = 0;
    for (size_t i = start; i < fac_cnt && fac[i] <= cur; ++i)
    {
        if (cur % fac[i] == 0)
            res += rec(cur / fac[i], i);   /* allow repeated factorials   */
    }
    return res;
}

/* public sequence function */
seq_t
a(idx_t n)
{
    init_factorials();
    return rec(n, 0);
}
