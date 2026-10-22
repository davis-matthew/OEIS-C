/*
 * A332512  Numbers k such that phi(k) == 0 (mod 12), where phi is the Euler
 *          totient function (A000010).
 * https://oeis.org/A332512
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 104, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   calc_phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A332512 – numbers k such that φ(k) == 0 (mod 12).
    The function generates the sequence by searching k in increasing
    order and collecting the values where the Euler totient is a
    multiple of 12.  Results are memoised so that any call order
    (not necessarily increasing) remains fast. */

/*  We need the Euler‑phi routine supplied in ../utils/calc_phi.h.    */
#include "../utils/calc_phi.h"
#include <stdlib.h>

/*  Helper that works both in C and C++  */
static seq_t *realloc_seq(seq_t *p, size_t newcap)
{
#if defined(__cplusplus)
    return reinterpret_cast<seq_t*>(realloc(p, newcap * sizeof(seq_t)));
#else
    return (seq_t*)realloc(p, newcap * sizeof(seq_t));
#endif
}

/*  Return the n‑th term (1‑based index) of the sequence. */
seq_t a(idx_t n)
{
    /*  Static state keeps the search progress and result cache.  */
    static idx_t  checked = 2;          /* smallest integer not yet examined   */
    static seq_t *cache  = NULL;        /* array of found terms                */
    static size_t cap    = 0;           /* capacity of cache                   */
    static size_t len    = 0;           /* number of terms stored              */

    if (n == 0)        /*  undefined, guard against accidental use  */
        return 0;

    /*  Ensure the cache contains at least n terms.  */
    while (len < n)
    {
        seq_t k   = (seq_t)checked;          /* candidate integer              */
        seq_t phi = calc_phi(k);             /* Euler totient of k             */
        if (phi % (seq_t)12 == (seq_t)0)
        {
            /*  Grow the cache if necessary.  */
            if (len == cap)
            {
                size_t newcap = cap ? cap * 2 : 64;
                seq_t *tmp = realloc_seq(cache, newcap);
                /*  In the test harness realloc() is guaranteed to succeed.  */
                cache = tmp;
                cap   = newcap;
            }
            cache[len++] = k;                /* store the found term             */
        }
        ++checked;                           /* next integer to examine          */
    }

    /*  Return the requested term (offset 1).  */
    return cache[n - 1];
}
