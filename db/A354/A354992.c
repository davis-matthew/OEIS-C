/*
 * A354992  Number of divisors d of n for which A344005(d) < A344005(n), where
 *          A344005(n) is the smallest positive integer m such that n divides
 *          m*(m+1).
 * https://oeis.org/A354992
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} [ A344005(d) < A344005(n) ] where A344005(m) =
 *            smallest k>0 such that m | k*(k+1)
 * Range:     u8 n <= 16, u16 n <= 3008, u32 n <= 9897, u64 n <= 9897,
 *            u128 n <= 9897, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

static seq_t *f_cache = NULL;   /* cache for A344005 values   */
static size_t  f_cap   = 0;     /* current capacity of f_cache */
static idx_t  f_max    = 0;     /* largest index computed in f_cache */

/* Ensure that f_cache contains A344005(m) for all m <= n   */
static void ensure_cache(idx_t n)
{
    if (n <= f_max) return;                /* already computed */

    size_t needed = (size_t)n + 1;
    if (needed > f_cap) {
        size_t new_cap = needed * 2 + 16;
        seq_t *new_cache = (seq_t *)realloc(f_cache, new_cap * sizeof(seq_t));
        if (!new_cache) abort();           /* allocation failed */
        f_cache = new_cache;
        f_cap   = new_cap;
    }

    for (idx_t m = f_max + 1; m <= n; ++m) {
        if (m == 1) {                     /* A344005(1) = 1 */
            f_cache[1] = (seq_t)1;
            continue;
        }
        seq_t best = 0;
        for (idx_t k = 1; k < m; ++k) {
            seq_t prod = (seq_t)k * (seq_t)(k + 1);
            if (prod % (seq_t)m == 0) {
                best = k;
                break;
            }
        }
        f_cache[m] = best;
    }
    f_max = n;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* safety; offset starts at 1 */

    ensure_cache(n);
    seq_t fn = f_cache[n];
    seq_t count = 0;

    /* factor n */
    idx_t temp = n;
    struct { idx_t p; int exp; } fac[10];
    size_t fc = 0;
    for (idx_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            int e = 0;
            while (temp % p == 0) { temp /= p; ++e; }
            fac[fc].p = p;
            fac[fc].exp = e;
            ++fc;
        }
    }
    if (temp > 1) {
        fac[fc].p = temp;
        fac[fc].exp = 1;
        ++fc;
    }

    /* generate all divisors of n */
    idx_t divs[256];
    size_t dcnt = 1;
    divs[0] = 1;
    for (size_t i = 0; i < fc; ++i) {
        idx_t prime = fac[i].p;
        int exp    = fac[i].exp;
        size_t old_dcnt = dcnt;
        for (size_t j = 0; j < old_dcnt; ++j) {
            idx_t base = divs[j];
            idx_t mult = 1;
            for (int k = 1; k <= exp; ++k) {
                mult *= prime;
                divs[dcnt++] = base * mult;
            }
        }
    }

    for (size_t i = 0; i < dcnt; ++i) {
        idx_t d = divs[i];
        if (f_cache[d] < fn) ++count;
    }
    return count;
}
