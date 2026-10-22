/*
 * A302417  Number of 4Xn 0..1 arrays with every element equal to 0, 1, 2 or 5
 *          horizontally, diagonally or antidiagonally adjacent elements, with
 *          upper left element zero.
 * https://oeis.org/A302417
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) -3*a(n-2) +6*a(n-3) -6*a(n-4) +3*a(n-5)
 *            -6*a(n-6) +a(n-7) -14*a(n-8) +9*a(n-9) +4*a(n-10) +2*a(n-11)
 *            -4*a(n-12) +2*a(n-13) +8*a(n-15) -4*a(n-16) a(1) = 8, a(2) = 81,
 *            a(3) = 120, a(4) = 259, a(5) = 698, a(6) = 1650, a(7) = 3655,
 *            a(8) = 8544, a(9) = 20246, a(10) = 47274, a(11) = 110...
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 23, u64 n <= 49, u128 n <= 101,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init21[21] = {
    (seq_t)8,      (seq_t)81,   (seq_t)120,  (seq_t)259,
    (seq_t)698,    (seq_t)1650, (seq_t)3655,  (seq_t)8544,
    (seq_t)20246, (seq_t)47274,(seq_t)110507,(seq_t)259419,
    (seq_t)607197,(seq_t)1420291,(seq_t)3328378,(seq_t)7799749,
    (seq_t)18264688,(seq_t)42776207,(seq_t)100206573,
    (seq_t)234721284,(seq_t)549779626
};

static seq_t compute_term(const seq_t *a, size_t i)
{
    /* recurrence valid for i >= 22 (n > 21) */
    return
        (                 (seq_t)3 * a[i-1]
         - (seq_t)3 * a[i-2]
         + (seq_t)6 * a[i-3]
         - (seq_t)6 * a[i-4]
         + (seq_t)3 * a[i-5]
         - (seq_t)6 * a[i-6]
         + (seq_t)1 * a[i-7]
         - (seq_t)14 * a[i-8]
         + (seq_t)9 * a[i-9]
         + (seq_t)4 * a[i-10]
         + (seq_t)2 * a[i-11]
         - (seq_t)4 * a[i-12]
         + (seq_t)2 * a[i-13]
         + (seq_t)8 * a[i-15]
         - (seq_t)4 * a[i-16] );
}

seq_t a(idx_t n)
{
    if (n == 0) {
        return 0; /* undefined for n < 1, but we return 0 for safety */
    }

    static seq_t *cache = NULL;      /* cache[1] .. cache[max_n] */
    static size_t max_n = 0;         /* highest index computed so far */

    /* initialise cache with the first 21 terms */
    if (max_n == 0) {
        cache = (seq_t *)malloc((21 + 1) * sizeof(seq_t)); /* discard index 0 */
        if (!cache) {
            return 0; /* allocation failure – undefined behaviour */
        }
        for (size_t i = 1; i <= 21; ++i) {
            cache[i] = init21[i-1];
        }
        max_n = 21;
    }

    /* if the requested term is already computed, return it */
    if ((size_t)n <= max_n) {
        return cache[n];
    }

    /* extend cache up to n */
    size_t new_max = (size_t)n;
    seq_t *tmp = (seq_t *)realloc(cache, (new_max + 1) * sizeof(seq_t));
    if (!tmp) {
        return 0; /* allocation failure – undefined behaviour */
    }
    cache = tmp;

    /* compute new terms using the recurrence */
    for (size_t i = max_n + 1; i <= new_max; ++i) {
        cache[i] = compute_term(cache, i);
    }
    max_n = new_max;

    return cache[n];
}
