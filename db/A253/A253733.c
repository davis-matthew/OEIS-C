/*
 * A253733  Number of (n+2)X(1+2) 0..1 arrays with every 2X2 and 3X3 subblock
 *          diagonal sum minus antidiagonal sum unequal to its neighbors
 *          horizontally and vertically.
 * https://oeis.org/A253733
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-2) + 15*a(n-3) + 44*a(n-4) + 26*a(n-5) + 36*a(n-6) -
 *            241*a(n-7) - 190*a(n-8) - 589*a(n-9) - 530*a(n-10) - 920*a(n-11)
 *            - 1248*a(n-12) - 800*a(n-13) - 384*a(n-14) for n > 15. Initial
 *            values: a(1)=218, a(2)=720, a(3)=2212, a(4)=6932, a(5)=22236,
 *            a(6)=70372, a(7)=222820, a(8)=706884, a(9)=...
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 15, u64 n <= 34, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* static cache for computed values */
static seq_t *cache = NULL;
static size_t cap = 0;        /* allocation size of cache */
static idx_t computed_max = 0;/* highest index already populated */

/* pre‑computed initial terms (index 0 unused) */
static const uint64_t init_vals64[16] = {
    0ULL,                     /* a(0) – unused */
    218ULL, 720ULL, 2212ULL, 6932ULL, 22236ULL, 70372ULL,
    222820ULL, 706884ULL, 2242532ULL, 7110804ULL,
    22555544ULL, 71517456ULL, 226861460ULL,
    719514384ULL, 2281956628ULL
};

/* ensure cache can address indices up to n inclusive */
static void ensure_cache(idx_t n)
{
    if ((idx_t)cap > n) return;          /* already enough slots */
    size_t need = (size_t)n + 1;         /* indices 0..n inclusive */
    size_t newcap = cap ? cap : 16;
    while (newcap < need) newcap <<= 1;  /* grow by powers of two */

    seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
    if (!tmp) abort();                   /* out of memory */
    if (newcap > cap) {
        /* initialise newly allocated part to zero */
        for (size_t i = cap; i < newcap; ++i) tmp[i] = (seq_t)0;
    }
    cache = tmp;
    cap   = newcap;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* not defined for n=0, give 0 */

    if (n <= 15) {
        /* small values are directly taken from the table */
        return (seq_t)init_vals64[(size_t)n];
    }

    ensure_cache(n);

    /* compute entries that have not yet been computed,
       starting after the highest already computed index */
    for (size_t i = (size_t)(computed_max + 1); i <= (size_t)n; ++i) {
        if (i <= 15) {
            cache[i] = (seq_t)init_vals64[i];
        } else {
            seq_t res = cache[i - 2];
            res += ((seq_t)15)  * cache[i - 3];
            res += ((seq_t)44)  * cache[i - 4];
            res += ((seq_t)26)  * cache[i - 5];
            res += ((seq_t)36)  * cache[i - 6];
            res -= ((seq_t)241) * cache[i - 7];
            res -= ((seq_t)190) * cache[i - 8];
            res -= ((seq_t)589) * cache[i - 9];
            res -= ((seq_t)530) * cache[i -10];
            res -= ((seq_t)920) * cache[i -11];
            res -= ((seq_t)1248)* cache[i -12];
            res -= ((seq_t)800) * cache[i -13];
            res -= ((seq_t)384) * cache[i -14];
            cache[i] = res;
        }
    }
    computed_max = n;
    return cache[n];
}
