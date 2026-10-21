/*
 * A276465  Divisors of 16450.
 * https://oeis.org/A276465
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   a(n) = the n-th divisor of 16450 in increasing order (OEIS
 *            A276465)
 * Range:     u8 n <= 7, u16 n <= 24, u32 n <= 24, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   factor_u64, seq_alloc, seq_cmp, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Divisors of 16450 are: 1,2,5,7,10,14,25,35,47,50,70,94,175,235,329,350,
 * 470,658,1175,1645,2350,3290,8225,16450 (24 terms).  The function generates
 *  the list on first call and then returns the n‑th element.  If n is out of
 *  range (n > 24) it returns 0.  The routine is O(1) after the
 *  one‑time initialization; all operations use only integer arithmetic, no
 *  floating point, and rely on the sequence utilities supplied by the build.
 */

#include <stdlib.h>
#include "../utils/factor_u64.h"   /* factor_u64 */
#include "../utils/seq_alloc.h"    /* seq_alloc */
#include "../utils/seq_mul.h"     /* seq_mul */
#include "../utils/seq_cmp.h"     /* seq_cmp */

seq_t a(idx_t n)
{
    /* Static cache for divisors of 16450 */
    static seq_t *divs = NULL;
    static size_t divs_cnt = 0;

    /* On first call, compute all divisors of 16450 */
    if (!divs) {
        /* Prime factorisation 16450 = 2 * 5^2 * 7 * 47  */
        uint64_t primes[64];
        uint64_t exp_raw[64];
        size_t pf_cnt = 0;
        factor_u64(16450ULL, primes, exp_raw, &pf_cnt);

        /* Compute total number of divisors: product (exp+1) */
        size_t total = 1;
        for (size_t i = 0; i < pf_cnt; ++i) {
            total *= (size_t)(exp_raw[i] + 1);
        }
        /* Allocate array for all divisors */
        divs = seq_alloc(total);
        if (!divs) {
            return 0;   /* allocation failure – unlikely for this size */
        }

        /* Generate divisors iteratively */
        size_t cur_cnt = 1;   /* we already have divisor 1 */
        divs[0] = (seq_t)1;

        for (size_t i = 0; i < pf_cnt; ++i) {
            seq_t p = (seq_t)primes[i];
            size_t e = (size_t)exp_raw[i];
            size_t old_cnt = cur_cnt;
            seq_t p_pow = (seq_t)1;
            for (size_t k = 1; k <= e; ++k) {
                /* p_pow = p_pow * p */
                p_pow = seq_mul(p_pow, p);
                /* Multiply existing divisors by p_pow */
                for (size_t j = 0; j < old_cnt; ++j) {
                    divs[cur_cnt++] = seq_mul(divs[j], p_pow);
                }
            }
        }
        divs_cnt = cur_cnt;

        /* Sort divisors in increasing order */
        qsort(divs, divs_cnt, sizeof(seq_t), seq_cmp);
    }

    /* Return the requested divisor (1‑based index) */
    if (n == 0 || (size_t)(n - 1) >= divs_cnt) {
        return 0; /* out of range */
    }
    return divs[(size_t)(n - 1)];
}
